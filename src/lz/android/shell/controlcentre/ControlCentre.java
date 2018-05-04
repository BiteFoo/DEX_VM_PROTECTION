package lz.android.shell.controlcentre;

import com.google.common.io.ByteStreams;
import lz.android.protect.EncodeSection;
import lz.android.protect.encodeFunc;
import lz.android.shell.base.Common;
import lz.android.shell.base.TypeDescription;
import lz.android.shell.base.Utils;
import lz.android.shell.base.ZipHelper;
import lz.android.shell.base.helper.AndroidManifestHelper;
import lz.android.shell.base.helper.CommandHelper;
import lz.android.shell.separator.InstructionInsert01;
import lz.android.shell.separator.Separator;
import lz.android.shell.separator.SeparatorOption;
import org.apache.log4j.Logger;
import org.jf.dexlib2.DexFileFactory;
import org.jf.dexlib2.dexbacked.raw.RawDexFile;

import java.io.*;
import java.nio.file.Files;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;


public class ControlCentre {

    private final static Logger log = Logger.getLogger(ControlCentre.class);

    private ControlCentreOption mOpt;
    private File mApkUnpackDir;

    public ControlCentre(ControlCentreOption opt) throws IOException {
        mOpt = opt;
        prepare();
    }
    /**
     * 做一些准备工作。
     */
    private void prepare() throws IOException {
       // Files.createTempDirectory()
        // 创建工作目录。
        mOpt.workspace = Files.createTempDirectory(mOpt.outDir.toPath(), "advmp").toFile();
        log.info("workspack:" + mOpt.workspace);

        mApkUnpackDir = new File(mOpt.workspace, "apk");
        mApkUnpackDir.mkdir();

        // 将整个apk文件解压。
        ZipHelper.unZip(mOpt.apkFile.getAbsolutePath(), mApkUnpackDir.getAbsolutePath());

    }





    /**
     * 加壳。
     *
     * @return
     */
    public boolean shell(String ProtectClassName,String ProtectMethodName,String InputProtectClassName) {
        boolean bRet = false;
        try {
            // 插入指令。
            TypeDescription classDesc = AndroidManifestHelper.findFirstClass(new File(mApkUnpackDir, "AndroidManifest.xml"),InputProtectClassName);
            InstructionInsert01 instructionInsert01 = new InstructionInsert01(new File(mApkUnpackDir, "classes.dex"), classDesc);
            instructionInsert01.insert();
            log.info("insert done!!!");

            // ***********运行抽离器。
            runSeparator(ProtectClassName,ProtectMethodName);

            // 从template目录中拷贝jni文件。
            copyJniFiles();

            // ********更新jni文件的内容。
            updateJniFiles();

            // 编译native代码。
            buildNative();

            // 将libs目录重命名为lib。
            mOpt.libDir = new File(mOpt.jniDir.getParentFile(), "lib");
            new File(mOpt.jniDir.getParentFile(), "libs").renameTo(mOpt.libDir);

            // 移动yc文件。
            File assetsDir = new File(mApkUnpackDir, "assets");
            if (!assetsDir.exists()) {
                assetsDir.mkdir();
            }
            File newYcFile = new File(assetsDir, "classes.nisl");
            Files.move(mOpt.outYcFile.toPath(), newYcFile.toPath());

            // 移动classes.dex文件。
            System.out.println("mOpt.outYcFile.getParent()"+mOpt.outYcFile.getParent());
            Utils.copyFile(new File(mOpt.outYcFile.getParent(), "classes.dex").getAbsolutePath(), new File(mApkUnpackDir, "classes.dex").getAbsolutePath());
            // 拷贝lib目录。
            Utils.copyFolder(mOpt.libDir.getAbsolutePath(), mApkUnpackDir.getAbsolutePath() + File.separator + "lib");

            //TODO 加密生成的so文件中的关键函数
           // encryption();

            //删除META—INF文件夹用于重签名
            File metaFile =new File(mApkUnpackDir.getAbsolutePath()+File.separator+"META-INF");
            Utils.forceDelete(metaFile);

            // 打包。
            String name = mOpt.apkFile.getName();
            name = name.substring(0, name.lastIndexOf('.'));
            File outApkFile = new File(mOpt.outDir, name + ".shelled.apk");
            log.info( " outApkFile.getAbsolutePath()"+outApkFile.getAbsolutePath());
            ZipHelper.doZip(mApkUnpackDir.getAbsolutePath(), outApkFile.getAbsolutePath());

            //删除临时文件
           // Utils.forceDelete(mOpt.workspace);

            //重签名
            reSig();

            bRet = true;
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return bRet;
    }

    /**
     * 运行抽取器。
     *
     * @return
     * @throws IOException
     */
    private boolean runSeparator(String ProtectClassName,String ProtectMethodName) throws IOException {
        SeparatorOption opt = new SeparatorOption();
        opt.dexFile = new File(mApkUnpackDir, "classes.dex");
        File outDir = new File(mOpt.workspace, "separator");
        opt.outDexFile = new File(outDir, "classes.dex");
        opt.outNISLFile = mOpt.outYcFile = new File(outDir, "classes.nisl");
        opt.outCPFile = mOpt.outYcCPFile = new File(outDir, "dexvmp.cpp");
        opt.outHFile=mOpt.outYchFile=new File(outDir,"avmp.h");
        opt.dexBuf=mOpt.dexBuf;
        //opt.whichSDK=Math.random()

        Separator separator = new Separator(opt);
        return separator.run(ProtectClassName,ProtectMethodName);
    }

    /**
     * 对生成的so文件中的关键函数加密保护
     */
    private boolean encryption()
    {
        File parent=new File(mApkUnpackDir.getAbsolutePath() + File.separator + "lib"+File.separator);
        if(parent.exists())
        {
           for(File childFile: parent.listFiles())
           {
               String sopath=childFile.getAbsolutePath()+File.separator+"lib"+Common.SO_NAME+".so";
              // EncodeSection encode=new EncodeSection(sopath,"_Z23NISLvmInterpretPortableP6YcFileiP7_JNIEnvP8_jobjectz");
               encodeFunc encode=new encodeFunc(sopath,"_Z23NISLvmInterpretPortableP6YcFileiP7_JNIEnvP8_jobjectz");
              boolean isEncrypt= encode.encode();
              if(isEncrypt)
                  log.info(sopath +"encrypt success");
           }
        }
      return true;
    }

    /**
     * 将template中的jni目录拷贝到工作目录。
     *
     * @throws IOException
     */
    private void copyJniFiles() throws IOException {
        // TODO 这里写死了。
        File jniTemplateDir = new File(System.getProperty("user.dir") + File.separator + "Tool" + File.separator + "template" + File.separator + "jni");
        mOpt.jniDir = new File(mOpt.workspace, "jni");
        log.info("old jniTemplateDir"+jniTemplateDir.getAbsolutePath());

        Utils.copyFolder(jniTemplateDir.getAbsolutePath(), mOpt.jniDir.getAbsolutePath());
    }

    /**
     * 更新jni目录中的文件。
     */
    private void updateJniFiles() throws IOException {
        File file;
        File tmpFile;
        StringBuffer sb = new StringBuffer();

        // 更新avmp.cpp文件中的内容。
        try (BufferedReader reader = new BufferedReader(new FileReader(mOpt.outYcCPFile))) {
            String line = null;
            while (null != (line = reader.readLine())) {
                sb.append(line);
                sb.append(System.getProperty("line.separator"));//行分隔符
            }
        }

        file = new File(mOpt.jniDir.getAbsolutePath() + File.separator + "advmpc" + File.separator + "avmp.cpp");
        tmpFile = new File(mOpt.jniDir.getAbsolutePath() + File.separator + "advmpc" + File.separator + "avmp.cpp" + ".tmp");
        try (BufferedReader reader = new BufferedReader(new FileReader(file));
             BufferedWriter writer = new BufferedWriter(new FileWriter(tmpFile))) {
            String line = null;
            while (null != (line = reader.readLine())) {
                if ("#ifdef _AVMP_DEBUG_".equals(line)) {
                    writer.write("#if 0");
                    writer.newLine();
                } else if ("//+${replaceAll}".equals(line)) {
                    writer.write(sb.toString());
                } else {
                    writer.write(line);
                    writer.newLine();
                }
            }
        }
        file.delete();
        tmpFile.renameTo(file);
        sb.delete(0, sb.length());

        //拷贝h文件到jni目录
        Utils.copyFile(mOpt.outYchFile.getAbsolutePath(),mOpt.jniDir.getAbsolutePath()+ File.separator + "advmpc" +File.separator+mOpt.outYchFile.getName());

    }
    /**
     * 对最终打包好的程序重签名
     * @throws FileNotFoundException
     */
    private void reSig() throws IOException {
        //拷贝打包程序到apk相同文件夹
        File sigjarDir = new File(System.getProperty("user.dir") + File.separator + "Tool" + File.separator + "Signer");
        String name = mOpt.apkFile.getName();
        name = name.substring(0, name.lastIndexOf('.'));
        File unsigapk = new File(mOpt.outDir, name + ".shelled.apk");
        File unsigapkDir =new File(unsigapk.getParent());
        String privateKey="";
        String publicKey="";
        String sigjar="";
        //循环将Signer文件夹下的所有文件复写到未签名文件夹下
        if (sigjarDir.exists())
        {
            if (sigjarDir.isDirectory())
            {
                for(String list:sigjarDir.list())
                {
                    File tmpfile =new File(unsigapkDir,list);
                    String tmpname=tmpfile.getName();
                   // Utils.copyFile(new File(sigjarDir,list).getAbsolutePath(),tmpfile.getAbsolutePath());
                   String subString =tmpname.substring(tmpname.lastIndexOf('.'),tmpname.length());
                    if (tmpname.substring(tmpname.lastIndexOf('.')+1,tmpname.length()).equals("pk8"))
                        privateKey=tmpname;
                    if (tmpname.substring(tmpname.lastIndexOf('.')+1,tmpname.length()).equals("pem"))
                        publicKey=tmpname;
                    if (tmpname.substring(tmpname.lastIndexOf('.')+1,tmpname.length()).equals("jar"))
                        sigjar=tmpname;

                }
            }else
                return;
        }else
            return;

        //调用签名jar进行重签名

       File privateKeyFile = new File(sigjarDir,privateKey);
        File publicKeyFile =new File(sigjarDir,publicKey);
        File sigjarFile =new File(sigjarDir,sigjar);
        if (privateKeyFile.exists()&&publicKeyFile.exists()&&sigjarFile.exists()) {
              File sigApk =new File(unsigapkDir,name+"_siged.apk");
            log.info("------ 开始签名 ------");
            CommandHelper.exec(new String[]{"java","-jar",sigjarFile.getAbsolutePath(),publicKeyFile.getAbsolutePath(),privateKeyFile.getAbsolutePath(),unsigapk.getAbsolutePath(),sigApk.getAbsolutePath()});
            log.info("------ 签名结束 ------");
            //重命名
            if(unsigapk.exists())
            {
                unsigapk.delete();
                sigApk.renameTo(unsigapk);
            }
        } else {
            throw new FileNotFoundException("签名文件不全，无法签名");
        }


    }


    /**
     * 编译native代码。
     * @throws FileNotFoundException
     */
    private void buildNative() throws FileNotFoundException {
        String ndkpathname ="NDK_HOME";
        String ndkpath="";
        Map<String,String> m = System.getenv();
        for(String key: m.keySet()){
            System.out.println(key+"  "+m.get(key));
            if ( ndkpathname.equals(key)) {
                ndkpath=m.get(key);
                break;
            }
        }
        File ndkDir= new File("D:\\env\\android-ndk-r10d");
        //log.info("ndkDir"+ndkDir);
        if (ndkDir.exists()) {
            String ndkPath = ndkDir.getAbsolutePath() + File.separator + "ndk-build";
            if (Utils.isWindowsOS()) {
                ndkPath += ".cmd";
            }
            log.info("------ 开始编译native代码 ------");
            // 编译native代码。
            CommandHelper.exec(new String[]{ndkPath, "NDK_PROJECT_PATH=" + mOpt.jniDir.getParent()});
            log.info("------ 编译结束 ------");
        } else {
            throw new FileNotFoundException("未能通过环境变量\"ndk\"找到ndk目录！这个环境变量可能未设置。");
        }
    }

}
