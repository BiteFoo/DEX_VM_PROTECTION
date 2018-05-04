package lz.android.shell.advmpformat;
//
import lz.android.shell.processins.DexFileFormat;
import lz.util.BitConverter;
import com.google.common.primitives.Ints;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
//
//
public class NISLFile {

    private File mOutFile;
    private Format mFormat;

    public NISLFile(File outFile, Format format) {
        mOutFile = outFile;
        mFormat = format;
    }

    /**
     * 写出NISL文件。
     */
    public void write() throws IOException {
        List<Byte> methods = writeMethod();
        List<Byte> separatorDatas = writeSeparatorData();


        // 创建文件头。
        mFormat.header = new Format.Header();
        mFormat.header.magic = Format.MAGIC;

        mFormat.header.size = Format.SIZE_HEADER;

        List<Format.AdvmpMethod> nislformat_methods = mFormat.methods;
        int methodFieldSize =0;//method结构体字段占用总大小，用来计算separdata的起始位置
        if (null == nislformat_methods || 0 == nislformat_methods.size()) {
            mFormat.header.methodSize = 0;
            mFormat.header.methodOffset = 0;
        } else {
            mFormat.header.methodSize = nislformat_methods.size();//method结构的多少
            mFormat.header.methodOffset = Format.SIZE_HEADER;
            for(int i=0;i<nislformat_methods.size();i++)
            {
                methodFieldSize+=4+4+4+4+4+4+nislformat_methods.get(i).name.size+nislformat_methods.get(i).definingClass.size+nislformat_methods.get(i).sig.size;
            }
        }
        if(null==mFormat.stringDatas)
        {
            mFormat.header.stringItemSize=0;
            mFormat.header.typeItemSize=0;
            mFormat.header.methodItemSize=0;
            mFormat.header.stringItemOffset=0;
            mFormat.header.typeItemOffest=0;
            mFormat.header.methodItemOffest=0;
            mFormat.header.fieldItemOffset=0;
        }else
            {
                int strSize=0;
                for(DexFileFormat.StringData sd: mFormat.stringDatas)
                {
                   strSize+= sd.stringItems.size;
                }
                mFormat.header.stringItemOffset=Format.SIZE_HEADER;
                //mFormat.header.stringItemSize=mFormat.stringDatas.size()*4+strSize;//用size大小的int数组存各个字符串的大小。
                mFormat.header.stringItemSize=mFormat.stringDatas.size();
                mFormat.header.typeItemOffest=mFormat.header.stringItemOffset+mFormat.stringDatas.size()*4+strSize;
                mFormat.header.typeItemSize=mFormat.typeItemDates.size();
                mFormat.header.methodItemOffest=mFormat.header.typeItemSize*4+mFormat.header.typeItemOffest;
                mFormat.header.methodItemSize=mFormat.methodItemDates.size();
                //11月30号新增
                mFormat.header.fieldItemOffset=mFormat.header.methodItemOffest+mFormat.methodItemDates.size()*12;
                mFormat.header.fieldItemSize=mFormat.fieldItemDates.size();
            }
        mFormat.header.separatorDataSize = mFormat.separatorDatas.size();
        //mFormat.header.separatorDataOffset = methodFieldSize+mFormat.header.methodItemOffest+mFormat.methodItemDates.size()*12;
        //11月30改
        mFormat.header.separatorDataOffset = methodFieldSize+mFormat.header.fieldItemOffset+mFormat.header.fieldItemSize*3*4;

        // 最后再生成文件头。
        List<Byte> header = writeHeader();
        List<Byte>  describeField =writeField();

        List<Byte> bytes = new ArrayList<>();
        bytes.addAll(header);
        bytes.addAll(methods);
        bytes.addAll(describeField);
        bytes.addAll(separatorDatas);

        byte[] arrBytes = new byte[bytes.size()];
        for (int i = 0; i < bytes.size(); i++) {
            arrBytes[i] = bytes.get(i);
        }
        try (BufferedOutputStream os = new BufferedOutputStream(new FileOutputStream(mOutFile))) {
            os.write(arrBytes);
        }
    }

    /**
     * 写四类描述字段
     * @return
     */
    private List<Byte> writeField() throws IOException {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        List<DexFileFormat.TypeItemDate> typeItemDates = mFormat.typeItemDates;
        List<DexFileFormat.StringData> stringDatas = mFormat.stringDatas;
        List<DexFileFormat.MethodItemDate> methodItemDates = mFormat.methodItemDates;
        List<DexFileFormat.FieldItemDate> fieldItemDates=mFormat.fieldItemDates;
        if(stringDatas==null){
            System.out.println("抽取失败，待保护方法的类名或方法名输入错误，请查证后输入，谢谢！");
            System.exit(0);
        }
        for (DexFileFormat.StringData stringData:stringDatas)
        {
            os.write(BitConverter.getBytes(stringData.stringItems.size));//先写每个字符串的大小
        }
        for (DexFileFormat.StringData stringData:stringDatas)
        {
           os.write(stringData.stringItems.str);
        }

        for(DexFileFormat.TypeItemDate typeItemDate:typeItemDates)
        {
            System.out.println(typeItemDate.typeItem);
            os.write(BitConverter.getBytes(typeItemDate.typeItem));
        }
        for(DexFileFormat.MethodItemDate methodItemDate:methodItemDates)
        {
            os.write(BitConverter.getBytes(methodItemDate.classNameIdx));
            os.write(BitConverter.getBytes(methodItemDate.protoIdx));
            os.write(BitConverter.getBytes(methodItemDate.methodNameIdx));
        }
        for(DexFileFormat.FieldItemDate fieldItemDate:fieldItemDates)
        {
            os.write(BitConverter.getBytes(fieldItemDate.classNameIdx));
            os.write(BitConverter.getBytes(fieldItemDate.typeIdx));
            os.write(BitConverter.getBytes(fieldItemDate.fieldNameIdx));
        }
        List<Byte> bytes = new ArrayList<>();
        for (byte b : os.toByteArray()) {
            bytes.add(b);
        }
        return bytes;

    }


    /**
     * 写文件头。
     * @throws IOException
     */
    private List<Byte> writeHeader() throws IOException {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        Format.Header header = mFormat.header;

        os.write(header.magic.getBytes());
        os.write(BitConverter.getBytes(header.size));
        os.write(BitConverter.getBytes(header.methodSize));
        os.write(BitConverter.getBytes(header.methodOffset));
        os.write(BitConverter.getBytes(header.separatorDataSize));
        os.write(BitConverter.getBytes(header.separatorDataOffset));
        os.write(BitConverter.getBytes(header.stringItemSize));
        os.write(BitConverter.getBytes(header.stringItemOffset));
        os.write(BitConverter.getBytes(header.typeItemSize));
        os.write(BitConverter.getBytes(header.typeItemOffest));
        os.write(BitConverter.getBytes(header.methodItemSize));
        os.write(BitConverter.getBytes(header.methodItemOffest));
        os.write(BitConverter.getBytes(header.fieldItemSize));
        os.write(BitConverter.getBytes(header.fieldItemOffset));

        List<Byte> bytes = new ArrayList<>();
        for (byte b : os.toByteArray()) {
            bytes.add(b);
        }
        return bytes;
    }

    /**
     * 向文件写入YcFormat.Method。
     * @throws IOException
     */
    private List<Byte> writeMethod() throws IOException {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        List<Format.AdvmpMethod> methods = mFormat.methods;
        if (null != methods) {
            for (Format.AdvmpMethod m : methods) {
                os.write(BitConverter.getBytes(m.methodIndex));
                os.write(BitConverter.getBytes(m.size));
                os.write(BitConverter.getBytes(m.accessFlag));
                os.write(m.definingClass.getBytes());
                os.write(m.name.getBytes());
                os.write(m.sig.getBytes());
            }
        }

        List<Byte> bytes = new ArrayList<>();
        for (byte b : os.toByteArray()) {
            bytes.add(b);
        }
        return bytes;
    }

    /**
     * 写SeparatorData。
     * @throws IOException
     */
    private List<Byte> writeSeparatorData() throws IOException {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        List<Format.SeparatorData> separatorDatas = mFormat.separatorDatas;
        for (Format.SeparatorData s : separatorDatas) {
            os.write(BitConverter.getBytes(s.methodIndex));
            os.write(BitConverter.getBytes(s.size));
            os.write(BitConverter.getBytes(s.accessFlag));
            os.write(BitConverter.getBytes(s.paramSize));
            os.write(BitConverter.getBytes(s.registerSize));
            os.write(BitConverter.getBytes(s.paramShortDesc.size));
            os.write(s.paramShortDesc.str);
            os.write(BitConverter.getBytes(s.instSize));
            os.write(BitConverter.getBytes(s.insts));
            // TODO ycformat 这里先不处理try...catch和调试信息。
        }

        List<Byte> bytes = new ArrayList<>();
        for (byte b : os.toByteArray()) {
            bytes.add(b);
        }
        return bytes;
    }

}
//public class NISLFile {
//
//    private File mOutFile;
//    private Format mFormat;
//
//    public NISLFile(File outFile, Format format) {
//        mOutFile = outFile;
//        mFormat = format;
//    }
//
//    /**
//     * 写出yc文件。
//     */
//    public void write() throws IOException {
//        //List<Byte> methods = writeMethod();
//        List<Byte> separatorDatas = writeSeparatorData();
//
//
//        // 创建文件头。
//        mFormat.header = new Format.Header();
//        mFormat.header.magic = Format.MAGIC;
//
//        mFormat.header.size = Format.SIZE_HEADER;
//
//        List<Format.AdvmpMethod> ycformat_methods = mFormat.methods;
//        if (null == ycformat_methods || 0 == ycformat_methods.size()) {
//            mFormat.header.methodSize = 0;
//            mFormat.header.methodOffset = 0;
//        } else {
//            mFormat.header.methodSize = ycformat_methods.size();
//            mFormat.header.methodOffset = Format.SIZE_HEADER;
//        }
//
//        mFormat.header.separatorDataSize = mFormat.separatorDatas.size();
//        mFormat.header.separatorDataOffset = Format.SIZE_HEADER;
//
//        // 最后再生成文件头。
//        List<Byte> header = writeHeader();
//
//        List<Byte> bytes = new ArrayList<>();
//        bytes.addAll(header);
//        //bytes.addAll(methods);
//        bytes.addAll(separatorDatas);
//
//        byte[] arrBytes = new byte[bytes.size()];
//        for (int i = 0; i < bytes.size(); i++) {
//            arrBytes[i] = bytes.get(i);
//        }
//        try (BufferedOutputStream os = new BufferedOutputStream(new FileOutputStream(mOutFile))) {
//            os.write(arrBytes);
//        }
//    }
//
//    /**
//     * 写文件头。
//     * @throws IOException
//     */
//    private List<Byte> writeHeader() throws IOException {
//        ByteArrayOutputStream os = new ByteArrayOutputStream();
//        Format.Header header = mFormat.header;
//
//
//        byte[] xxx = Ints.toByteArray(10);
//
//        os.write(header.magic.getBytes());
//        os.write(BitConverter.getBytes(header.size));
//        os.write(BitConverter.getBytes(header.methodSize));
//        os.write(BitConverter.getBytes(header.methodOffset));
//        os.write(BitConverter.getBytes(header.separatorDataSize));
//        os.write(BitConverter.getBytes(header.separatorDataOffset));
//
//        List<Byte> bytes = new ArrayList<>();
//        for (byte b : os.toByteArray()) {
//            bytes.add(b);
//        }
//        return bytes;
//    }
//
//    /**
//     * 向文件写入YcFormat.Method。
//     * @throws IOException
//     */
////    private List<Byte> writeMethod() throws IOException {
////        ByteArrayOutputStream os = new ByteArrayOutputStream();
////        List<YcFormat.AdvmpMethod> methods = mFormat.methods;
////        if (null != methods) {
////            for (YcFormat.AdvmpMethod m : methods) {
////                os.write(BitConverter.getBytes(m.methodIndex));
////                os.write(BitConverter.getBytes(m.size));
////                os.write(m.definingClass..getBytes());
////                os.write(m.name.getBytes());
////                os.write(m.sig.getBytes());
////            }
////        }
////
////        List<Byte> bytes = new ArrayList<>();
////        for (byte b : os.toByteArray()) {
////            bytes.add(b);
////        }
////        return bytes;
////    }
//
//    /**
//     * 写SeparatorData。
//     * @throws IOException
//     */
//    private List<Byte> writeSeparatorData() throws IOException {
//        ByteArrayOutputStream os = new ByteArrayOutputStream();
//        List<Format.SeparatorData> separatorDatas = mFormat.separatorDatas;
//        for (Format.SeparatorData s : separatorDatas) {
//            os.write(BitConverter.getBytes(s.methodIndex));
//            os.write(BitConverter.getBytes(s.size));
//            os.write(BitConverter.getBytes(s.accessFlag));
//            os.write(BitConverter.getBytes(s.paramSize));
//            os.write(BitConverter.getBytes(s.registerSize));
//            os.write(BitConverter.getBytes(s.paramShortDesc.size));
//            os.write(s.paramShortDesc.str);
//            os.write(BitConverter.getBytes(s.instSize));
//            os.write(BitConverter.getBytes(s.insts));
//            // TODO ycformat 这里先不处理try...catch和调试信息。
//        }
//
//        List<Byte> bytes = new ArrayList<>();
//        for (byte b : os.toByteArray()) {
//            bytes.add(b);
//        }
//        return bytes;
//    }
//
//}

