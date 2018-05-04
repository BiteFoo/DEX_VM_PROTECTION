package lz.android.shell.base.helper;

import com.google.common.io.ByteStreams;
import lz.android.shell.advmpformat.StringItem;
import lz.android.shell.base.Common;
import lz.android.shell.base.Utils;
import lz.android.shell.processins.DexFileFormat;
import lz.android.shell.processins.GetField;
import lz.android.shell.processins.Method;
import lz.android.shell.separator.InstructionInsert01;
import lz.android.shell.separator.Separator;
import lz.android.shell.separator.SeparatorOption;
import org.jf.dexlib2.DexFileFactory;
import org.jf.dexlib2.Opcode;
import org.jf.dexlib2.Opcodes;
import org.jf.dexlib2.analysis.MethodAnalyzer;
import org.jf.dexlib2.dexbacked.BaseDexBuffer;
import org.jf.dexlib2.dexbacked.DexBackedClassDef;
import org.jf.dexlib2.dexbacked.DexBackedDexFile;
import lz.android.dexlib2.helper.MethodHelper;
import org.jf.dexlib2.dexbacked.DexReader;
import org.jf.dexlib2.dexbacked.raw.*;
import org.jf.dexlib2.dexbacked.util.FixedSizeSet;
import org.jf.dexlib2.iface.ClassDef;
import org.jf.dexlib2.iface.DexFile;
import org.jf.dexlib2.iface.MethodImplementation;
import org.jf.dexlib2.iface.MethodParameter;
import org.jf.util.ExceptionWithContext;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import java.io.*;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Created by lz on 2017/10/30.
 */

public class InstHelper {
    public static DexBackedDexFile mDexFile;
    public static RawDexFile mRawDexFile;

    public InstHelper(SeparatorOption mOpt) {
        mDexFile = (DexBackedDexFile) Separator.mDexFile;
        File dexFile = mOpt.dexFile;
        try {
            InputStream in = new FileInputStream(dexFile);
            long fileLength = dexFile.length();
            mOpt.dexBuf = new byte[(int) fileLength];
            ByteStreams.readFully(in, mOpt.dexBuf);

        } catch (Exception e) {
            e.printStackTrace();

        }
        //TODO dexBUF
        mRawDexFile = new RawDexFile(mDexFile.getOpcodes(), mOpt.dexBuf);
        // mRawDexFile.getTypeIdItemOffset(11);

    }

    /**
     * 读取C语言中的uleb类型
     * 目的是解决整型数值浪费问题
     * 长度不固定，在1~5个字节中浮动
     *
     * @param srcByte
     * @param offset
     * @return
     */
    public static byte[] readUnsignedLeb128(byte[] srcByte, int offset) {
        List<Byte> byteAryList = new ArrayList<Byte>();
        byte bytes = Utils.copyByte(srcByte, offset, 1)[0];
        byte highBit = (byte) (bytes & 0x80);
        byteAryList.add(bytes);
        offset++;
        while (highBit != 0) {
            bytes = Utils.copyByte(srcByte, offset, 1)[0];
            highBit = (byte) (bytes & 0x80);
            offset++;
            byteAryList.add(bytes);
        }
        byte[] byteAry = new byte[byteAryList.size()];
        for (int j = 0; j < byteAryList.size(); j++) {
            byteAry[j] = byteAryList.get(j);
        }
        return byteAry;
    }

    public static StringItem getStringItem(int offset) {
        String[] strings = StringIdItem.getStrings(mRawDexFile);
        StringItem result = new StringItem();
        String currString = strings[offset];
        result.size = currString.length();
        result.str = currString.getBytes();
        result.strs = currString;
        return result;
    }

    public StringItem getNonMethodTypeItem(int offset) {
        String[] Types = TypeIdItem.getTypes(mRawDexFile);
        StringItem result = new StringItem();
        result.str = Types[offset].getBytes();
        result.size = result.str.length;
        return result;
    }

    public StringItem getTypeItem(int offset) {

        String[] Types = TypeIdItem.getTypes(mRawDexFile);
        StringItem result = new StringItem();
        String currType = Types[offset];
        result.str = currType.substring(1, currType.length() - 1).getBytes();//不需要L和分号
        result.size = result.str.length;
        return result;
    }

//    public StringItem getTypeItem1(int offset) {
//
//        String[] Types = TypeIdItem.getTypes(mRawDexFile);
//        StringItem result = new StringItem();
//        String currType = Types[offset];
//        result.str = currType.getBytes();//不需要L和分号
//        result.size = result.str.length;
//        return result;
//    }

    /**
     * 获取method的classname
     * Para method的编号
     */
    public StringItem getClassName(int classItemOffset) {

        return getTypeItem(classItemOffset);
    }

    /**
     * 获取method 的proto
     */
    public StringItem getProto(int protoIdx) {
        //String[] Protos =ProtoIdItem.getProtos(mRawDexFile);
        String currProto = ProtoIdItem.asString(mRawDexFile, protoIdx);
        StringItem result = new StringItem();
        result.strs=currProto;
        result.str = currProto.getBytes();
        result.size = currProto.length();
        return result;
    }

    /**
     * 获取method的name
     */
    public StringItem getMethodName(int methodIdx) {
        return getStringItem(methodIdx);
    }

    /**
     * function:查找filed_id_item里面的内容。
     */
    public GetField getFiledItem(int fieldIdx) {

        //fieldIdx=fieldIdx; fixed  不应该减一？  为什么有的需要有的不需要
        int fieldIdOffest = mDexFile.getFieldIdItemOffset(fieldIdx);
        GetField mgetfield = new GetField();
        short mClassNameIdx = (short) mDexFile.readUshort(fieldIdOffest);
        mgetfield.setClassNameIdx(mClassNameIdx); //2154
        short mTypeIdx = (short) mDexFile.readUshort(fieldIdOffest + 2);//2101
        int mNameIdx = mDexFile.readInt(fieldIdOffest + 4);
        mgetfield.setfiledNameIdx(mNameIdx);
        mgetfield.setTypeIdx(mTypeIdx);
        mgetfield.className = getClassName(mgetfield.getClassNameIdx());
        mgetfield.filed = getStringItem(mgetfield.getFieldNameIdx());//只有这个为int类型
        mgetfield.typeName = getNonMethodTypeItem(mgetfield.getTypeIdx());
        return mgetfield;
    }

    public Method getMethod(int methodIdx) {
        methodIdx = methodIdx;
        String str = MethodIdItem.asString(mRawDexFile, methodIdx);
        int methodFieldOffest = mDexFile.getMethodIdItemOffset(methodIdx);
        Method mMethod = new Method();
        short mClassNameIdx = (short) mDexFile.readUshort(methodFieldOffest);
        mMethod.setClassNameIdx(mClassNameIdx);
        short mProtoIdx = (short) mDexFile.readUshort(methodFieldOffest + 2);
        int mNameIdx = mDexFile.readInt(methodFieldOffest + 4);
        mMethod.setProtoIdx(mProtoIdx);
        mMethod.setMethodNameIdx(mNameIdx);
        mMethod.methodName = getMethodName(mMethod.getMethodNameIdx());
        mMethod.className = getClassName(mMethod.getClassNameIdx());
        mMethod.proto = getProto(mMethod.getProtoIdx());
        return mMethod;
    }
}

