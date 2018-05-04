package lz.android.protect;

import org.apache.log4j.Logger;
import  lz.android.protect.ElfType32.*;


public class EncodeSection {
    public static String encodeSectionName=".mytext";
    public String sofilename;
    private static  final Logger log =Logger.getLogger(EncodeSection.class);
    public static ElfType32 type_32 = new ElfType32();
    public EncodeSection(String sofilename)
    {
        this.sofilename=sofilename;
    }
    public  boolean encrypt()
    {
        byte[] fileByteArys=Utils.readFile(sofilename);
        if(fileByteArys==null)
        {
            log.info("read file byte failed..");
            return false;

        }
        /**
         * 1. 解析so文件
         * 2. 初始化AddSection中的一些信息
         * 3. AddSection
         */
        pareSo(fileByteArys);
        encodeSection(fileByteArys);
        Utils.saveFile("so/libdemos",fileByteArys);
        return true;
    }

    private static void pareSo(byte[] fileByteArys)
    {
        System.out.println("+++++++++++++++++++Elf Header+++++++++++++++++");
        parseHeader(fileByteArys, 0);
        System.out.println("header:\n"+type_32.hdr);

        int p_header_offset = Utils.byte2Int(type_32.hdr.e_phoff);
        parseProgramHeaderList(fileByteArys, p_header_offset);
    }

    private static void parseHeader(byte[] header,int offset)
    {
        if(null==header)
        {
            log.info("header is null");
            return;
        }

        type_32.hdr.e_ident = Utils.copyBytes(header, 0, 16);//魔数
        type_32.hdr.e_type = Utils.copyBytes(header, 16, 2);
        type_32.hdr.e_machine = Utils.copyBytes(header, 18, 2);
        type_32.hdr.e_version = Utils.copyBytes(header, 20, 4);
        type_32.hdr.e_entry = Utils.copyBytes(header, 24, 4);
        type_32.hdr.e_phoff = Utils.copyBytes(header, 28, 4);
        type_32.hdr.e_shoff = Utils.copyBytes(header, 32, 4);
        type_32.hdr.e_flags = Utils.copyBytes(header, 36, 4);
        type_32.hdr.e_ehsize = Utils.copyBytes(header, 40, 2);
        type_32.hdr.e_phentsize = Utils.copyBytes(header, 42, 2);
        type_32.hdr.e_phnum = Utils.copyBytes(header, 44,2);
        type_32.hdr.e_shentsize = Utils.copyBytes(header, 46,2);
        type_32.hdr.e_shnum = Utils.copyBytes(header, 48, 2);
        type_32.hdr.e_shstrndx = Utils.copyBytes(header, 50, 2);
    }

    /**
     * 解析程序头信息
     * @param header
     */
    public static void parseProgramHeaderList(byte[] header, int offset){
        int header_size = 32;//32个字节
        int header_count = Utils.byte2Short(type_32.hdr.e_phnum);//头部的个数
        byte[] des = new byte[header_size];
        for(int i=0;i<header_count;i++){
            System.arraycopy(header, i*header_size + offset, des, 0, header_size);
            type_32.phdrList.add(parseProgramHeader(des));
        }
    }

    private static  elf32_phdr parseProgramHeader(byte[] header){
        /**
         *  public int p_type;
         public int p_offset;
         public int p_vaddr;
         public int p_paddr;
         public int p_filesz;
         public int p_memsz;
         public int p_flags;
         public int p_align;
         */
        elf32_phdr phdr = new elf32_phdr();
        phdr.p_type = Utils.copyBytes(header, 0, 4);
        phdr.p_offset = Utils.copyBytes(header, 4, 4);
        phdr.p_vaddr = Utils.copyBytes(header, 8, 4);
        phdr.p_paddr = Utils.copyBytes(header, 12, 4);
        phdr.p_filesz = Utils.copyBytes(header, 16, 4);
        phdr.p_memsz = Utils.copyBytes(header, 20, 4);
        phdr.p_flags = Utils.copyBytes(header, 24, 4);
        phdr.p_align = Utils.copyBytes(header, 28, 4);
        return phdr;

    }

    private static void encodeSection(byte[] fileByteArys)
    {

        //读取String Section段
        System.out.println();

        int string_section_index = Utils.byte2Short(type_32.hdr.e_shstrndx);
        elf32_shdr shdr = type_32.shdrList.get(string_section_index);
        //获得字符串索引section的结构
        int size = Utils.byte2Int(shdr.sh_size);
        int offset = Utils.byte2Int(shdr.sh_offset);

        int mySectionOffset=0,mySectionSize=0;
        for(elf32_shdr temp : type_32.shdrList){
            int sectionNameOffset = offset+Utils.byte2Int(temp.sh_name);
            if(Utils.isEqualByteAry(fileByteArys, sectionNameOffset, encodeSectionName)){
                //这里需要读取section段然后进行数据加密
                mySectionOffset = Utils.byte2Int(temp.sh_offset);
                mySectionSize = Utils.byte2Int(temp.sh_size);
                byte[] sectionAry = Utils.copyBytes(fileByteArys, mySectionOffset, mySectionSize);
                for(int i=0;i<sectionAry.length;i++){
                    sectionAry[i] = (byte)(sectionAry[i] ^ 0xFF);
                }
                Utils.replaceByteAry(fileByteArys, mySectionOffset, sectionAry);
            }
        }

        //修改Elf Header中的entry和offset值
        int nSize = mySectionSize/4096 + (mySectionSize%4096 == 0 ? 0 : 1);
        byte[] entry ;
        entry = Utils.int2Byte((mySectionSize<<16) + nSize);
        Utils.replaceByteAry(fileByteArys, 24, entry);
        byte[] offsetAry ;
        offsetAry = Utils.int2Byte(mySectionOffset);
        Utils.replaceByteAry(fileByteArys, 32, offsetAry);
    }
}
