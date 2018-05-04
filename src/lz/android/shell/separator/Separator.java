package lz.android.shell.separator;

import lz.android.dexlib2.helper.MethodHelper;
import lz.android.shell.advmpformat.StringItem;
import lz.android.shell.advmpformat.NISLFile;
import lz.android.shell.advmpformat.Format;
import lz.android.shell.base.Common;
import lz.android.shell.separator.config.Config;
import lz.android.shell.separator.config.ConfigHelper;
import lz.android.shell.separator.config.ConfigParse;
import org.jf.dexlib2.AccessFlags;
import org.jf.dexlib2.DexFileFactory;
import org.jf.dexlib2.dexbacked.DexBackedMethod;
import org.jf.dexlib2.iface.ClassDef;
import org.jf.dexlib2.iface.DexFile;
import org.jf.dexlib2.iface.Method;
import org.jf.dexlib2.iface.MethodParameter;
import org.jf.dexlib2.immutable.ImmutableMethod;
import org.jf.dexlib2.immutable.ImmutableMethodParameter;
import org.jf.dexlib2.rewriter.*;
import lz.android.shell.processins.*;
import javax.annotation.Nonnull;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * 方法指令抽取器。
 */
public class Separator {
    public static DexFile mDexFile;
    private ConfigHelper mConfigHelper;
    private DexRewriter mDexRewriter;
    private SeparatorOption mOpt;

    private List<Method> mSeparatedMethod = new ArrayList<>();//mSepatedMethod用来记录整个被抽取的函数
    private List<Format.SeparatorData> mSeparatorData = new ArrayList<>();
    private String ProtectingClassName = null;
    private String ProtectingMethodName = null;

    /**
     * @param opt
     */
    public Separator(SeparatorOption opt) throws IOException {
        mOpt = opt;
        //mOpt.whichSDK=(int) (Math.random()*4);//随机从四套指令中选择一套
        mOpt.whichSDK=3;

        mDexFile = DexFileFactory.loadDexFile(opt.dexFile, Common.API); // 这个dex文件是已经修改过的，
        SeparatorRewriterModule srm =  new SeparatorRewriterModule();
        mDexRewriter = new SeparatorDexRewriter(srm);



        ConfigParse cfg   = new ConfigParse(opt.configFile);
        Config config = cfg.parse();
        //opt.configFile是空的!!!

        // 解析配置文件。
        mConfigHelper = new ConfigHelper(config);

        if (!mOpt.outDexFile.exists()) {
            mOpt.outDexFile.getParentFile().mkdirs();
        }
    }

    /**
     * 抽取方法指令。
     *
     * @return true：成功。false：失败。
     */
    public boolean run(String ProtectClassName,String ProtectMethodName) {

        boolean bRet = false;

        ProtectingClassName = ProtectClassName;
        ProtectingMethodName = ProtectMethodName;
        // 重写dex文件
         DexFile newDexFile = mDexRewriter.rewriteDexFile(mDexFile);
        try {
            // 将新dex输出到文件。
            DexFileFactory.writeDexFile(mOpt.outDexFile.getAbsolutePath(), newDexFile);

            // 写NISL文件。
            writeNISLFile();

            // 写c和h文件。
            writeCFileandH();

            bRet = true;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return bRet;
    }

    /**
     * 写NISL文件。
     *
     * @throws IOException
     */
    private void writeNISLFile() throws IOException {
        Format format = new Format();
        // TODO 没有对methods字段赋值。
        format.methods =new ArrayList<>();
        for(int i=0;i<mSeparatedMethod.size();i++)
        {
            Format.AdvmpMethod method =new Format.AdvmpMethod();
            method.accessFlag =mSeparatedMethod.get(i).getAccessFlags();
            method.definingClass = new StringItem();
            method.definingClass.str =mSeparatedMethod.get(i).getDefiningClass().getBytes();
            method.definingClass.size=method.definingClass.str.length;
            // method.methodIndex=0;
            method.name=new StringItem();
            method.name.str=mSeparatedMethod.get(i).getName().getBytes();
            method.name.size=method.name.str.length;
            method.sig=new StringItem();
            // method.sig=mSeparatedMethod.get(i).getParameters()
            //TODO 不写Method
            //format.methods.add(method);
        }
       // ToAdvmpMehod(format.methods);
        format.separatorDatas = mSeparatorData;
        format.stringDatas=ProcessIns.mstringDatas;
        format.typeItemDates=ProcessIns.mTypeItemDates;
        format.methodItemDates=ProcessIns.mMethodItemDates;
        format.fieldItemDates=ProcessIns.mfieldItemDates;
        NISLFile NISLFile = new NISLFile(mOpt.outNISLFile, format);
        NISLFile.write();
    }

    /**
     * 写C文件。
     */
    private void writeCFileandH() throws IOException {
        //TODO
        int whichSDK=mOpt.whichSDK;
        SeparatorHWriter separatorHWriter=new SeparatorHWriter(whichSDK,mOpt.outHFile);
        separatorHWriter.write();
        SeparatorCWriter separatorCWriter = new SeparatorCWriter(mOpt.outCPFile, mSeparatedMethod,whichSDK);
        separatorCWriter.write();
        System.out.println(mOpt.outCPFile.getAbsolutePath());
    }

    class SeparatorDexRewriter extends DexRewriter {

        public SeparatorDexRewriter(RewriterModule module) {
            super(module);//调用重写后的
        }

        @Nonnull
        @Override
        public DexFile rewriteDexFile(@Nonnull DexFile dexFile) {
            return super.rewriteDexFile(dexFile);
        }
    }

    class SeparatorRewriterModule extends RewriterModule {
        @Nonnull
        @Override
        public Rewriter<ClassDef> getClassDefRewriter(@Nonnull Rewriters rewriters) {
            return new ClassDefRewriter(rewriters) {
                @Nonnull
                @Override
                public ClassDef rewrite(@Nonnull ClassDef classDef) {
                    return super.rewrite(classDef);
                }
            };
        }

      //TODO 暂时修改
        int flag=2;
        @Nonnull
        @Override
        public Rewriter<Method> getMethodRewriter(Rewriters rewriters) {
            //
            return new MethodRewriter(rewriters) {
                @Nonnull
                @Override
                public Method rewrite(@Nonnull Method value) {
                    System.out.println("methodName "+value.getName()+" Parameters "+value.getParameters()+" tDefiningClass "+value.getDefiningClass()+" ReturnType "+value.getReturnType());
//                   String addMethod ="separatorTest";
//                    if(value.getName().equals(addMethod))
//                    System.out.println("separatorTest method find");

                    if (mConfigHelper.isValid(value,ProtectingClassName,ProtectingMethodName)) {
                        mSeparatedMethod.add(value);//记录被分离出来的Method
                        System.out.println(" separatorTest 的代码抽取中");
                        Format.SeparatorData separatorData = new Format.SeparatorData();
                        separatorData.methodIndex = mSeparatorData.size();//标识是第几个被抽取的函数


                        separatorData.accessFlag = value.getAccessFlags();
                        separatorData.paramSize = value.getParameters().size();
                        //获取寄存器的大小
                        separatorData.registerSize = value.getImplementation().getRegisterCount();

                        //方法的短类型描述（使用部分前缀描述）
                        separatorData.paramShortDesc = new StringItem();
                        separatorData.paramShortDesc.str = MethodHelper.genParamsShortDesc(value).getBytes();
                        separatorData.paramShortDesc.size = separatorData.paramShortDesc.str.length;
                        System.out.println("separatorData.paramShortDesc.str   " + separatorData.paramShortDesc.str.toString());
                        System.out.println("separatorData.paramShortDesc.size   " + separatorData.paramShortDesc.size);
                        separatorData.insts = MethodHelper.getInstructions((DexBackedMethod) value);
                        System.out.println("方法指令如下");
                        for (short ins : separatorData.insts) {
                            int shortValue = ins & 0x7fff + (Math.abs(ins >> 15) << 15);
                            String shortValue1 = Integer.toHexString(shortValue);
                            System.out.print(shortValue1 + " ");
                        }
                        System.out.println();
                        ProcessIns pi = new ProcessIns(separatorData, mOpt);
                        separatorData.insts = pi.processing();//重新对insts赋值
                        separatorData.instSize = separatorData.insts.length;
                        separatorData.size = 4 + 4 + 4 + 4 + 4 + separatorData.paramShortDesc.size + 4 + (separatorData.instSize * 2) + 4;


                        mSeparatorData.add(separatorData);//每轮回一次 加一次

                        // 下面这么做的目的是要把方法的name删除，否则生成的dex安装的时候会有这个错误：INSTALL_FAILED_DEXOPT。
                        List<? extends MethodParameter> oldParams = value.getParameters();
                        List<ImmutableMethodParameter> newParams = new ArrayList<>();
                        for (MethodParameter mp : oldParams) {
                            newParams.add(new ImmutableMethodParameter(mp.getType(), mp.getAnnotations(), null));
                        }
                        //new ImmutableMethod(value.getDefiningClass(), value.getName(), newParams, value.getReturnType(), value.getAccessFlags() | AccessFlags.NATIVE.getValue(), value.getAnnotations(), null);

                        //如果是的话返回一个新的方法，如果不是的话返回父类的rewrite
                        return new ImmutableMethod(value.getDefiningClass(), value.getName(), newParams, value.getReturnType(), value.getAccessFlags() | AccessFlags.NATIVE.getValue(), value.getAnnotations(), null);
                    }

                    return super.rewrite(value);
                }
            };
        }
    }

}
