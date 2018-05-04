package lz.android.shell.processins;

import lz.android.shell.advmpformat.Format;
import lz.android.shell.advmpformat.StringItem;
import lz.android.shell.base.helper.InstHelper;
import lz.android.shell.separator.SeparatorOption;
import org.apache.log4j.Logger;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by lz on 2017/10/26.
 */

public class ProcessIns {
    /**
     * 保存被抽取到的指令
     */
    short[] ins;
    Format.SeparatorData separatorData;
    public int whichSDK = 0;

    float[] ins32;//float为32位

    long[] ins64;//long为64位
    /**
     * 文件信息
     */
    SeparatorOption mOpt;
    /**
     *
     */
    final short OFFSETLENGTH = 1;//偏移一个short
    public static List<DexFileFormat.MethodItemDate> mMethodItemDates;
    public static List<DexFileFormat.TypeItemDate> mTypeItemDates;
    public static List<DexFileFormat.StringData> mstringDatas;
    public static List<DexFileFormat.FieldItemDate> mfieldItemDates;
    public static List<ArrayDate> mArrayDate;
    //此结构用来存储new——instance的引用，第一个Integer存储引用位置，后一个Integer用来存储替换的数值
    //public static Map<Integer ,Integer> mInstance;
    public static List<Instance> mInstance;


    public ProcessIns(Format.SeparatorData separatorData, SeparatorOption mOpt) {
        this.separatorData = separatorData;
        this.separatorData.registerSize = separatorData.registerSize;
        this.separatorData.paramSize = separatorData.paramSize;
        this.separatorData.accessFlag = separatorData.accessFlag;
        this.ins = separatorData.insts;
        this.mOpt = mOpt;
        short[] insts;
        mMethodItemDates = new ArrayList<>();
        mTypeItemDates = new ArrayList<>();
        mstringDatas = new ArrayList<>();
        mfieldItemDates = new ArrayList<>();
        mArrayDate = new ArrayList<>();
        mInstance = new ArrayList<>();


    }


    private static final Logger logger = Logger.getLogger(ProcessIns.class);
    private List<Method> mMethod = new ArrayList<>();
    private List<Method> mGetField = new ArrayList<>();


    public static String short2FormatString(short num) {

        return String.format("%04x", num);
    }

    public static String short2FormatString(long num) {

        return String.format("%04x", num);
    }

    public static int INST_A(short[] ins, int pos)//获取目的寄存器
    {


        return ((ins[pos] >> 12) & 0xf);
    }

    public static int INST_B(short[] ins, int pos)//获取源寄存器
    {
        return (ins[pos] >> 8) & 0xf;

    }

    public static int INST_AA(short[] ins, int pos) {
        return ins[pos] >> 8;
    }

    public static int FETCH(short[] ins, int pos) {
        return ins[pos + 1];
    }

    public static int FETCHINSTA(short[] ins, int pos) {//获取32位的后16位源寄存器的后八位
        return ins[pos + 1] & 0xff;
    }//16位

    public static int FETCHINSTB(short[] ins, int pos) {//获取32位的后16位源寄存器的前八位
        return (ins[pos + 1] >> 8) & 0xff;
    }//16位

    public static long FETCHWIDE(short[] ins, int pos) {//获取32位源寄存器
        return (ins[pos + 1]) << 16 + ins[pos + 2];
    }

    //32位
    public static long FETCH64(short[] ins, int pos) {//获取64位源寄存器
        return (ins[pos + 1]) << 32 + ins[pos + 2] << 16 + ins[pos + 3];
    }
    //64位


    public static String getInvokePara(short[] ins, int pos) {
        int paraSize = ins[pos] >> 12;
        int tmpPara = (ins[pos] >> 8) & 0xf;
        StringBuilder builder = new StringBuilder("{ ");
        int[] paraArry = new int[paraSize];
        if (paraSize < 5) {
            for (int i = 0; i < paraSize; i++) {
                paraArry[i] = (ins[pos + 2] >> (4 * i)) & 0xf;
                builder.append("v" + paraArry[i] + " ");
            }
            builder.append(" }");

        } else {
            for (int i = 0; i < paraSize - 1; i++) {
                paraArry[i] = (ins[pos + 2] >> (4 * i)) & 0xf;
                builder.append("v" + paraArry[i]);
            }
            builder.append("v" + tmpPara + " }");
        }
        return builder.toString();
    }

    public static String getInvokeRange(short[] ins, int pos) {
        int paraCount = ins[pos] >> 8;//拿到是4
        int firstPara = ins[pos + 2];
        int lastPara = firstPara + paraCount - 1;
        if (1 == paraCount)
            return "v" + firstPara;
        else
            return "{ " + "v" + firstPara + " v" + lastPara + " }";
    }

    /**
     * function:判断stringDatas有没有重复；
     *
     * @param stringItem
     * @return
     */
    private static int findExistString(StringItem stringItem) {//返回值为数组下标，或者为无效值 代表不存在

        for (int i = 0; i < mstringDatas.size(); i++) {
            if (mstringDatas.get(i).stringItems.str.length == stringItem.str.length) {
                if (Arrays.equals(mstringDatas.get(i).stringItems.str, stringItem.str)) {
                    return i;//返回第i个与stringItem相等
                }
            }
        }
        return -1;//遍历结束。并没有与之相等的。
    }

    /**
     * function:判断TypeItemDatas有没有重复；
     */
    private static int findExistTypeItem(int typeItemDate) {//返回值为数组下标，或者为无效值 代表不存在

        for (int i = 0; i < mTypeItemDates.size(); i++) {

            if (mTypeItemDates.get(i).typeItem == typeItemDate) {
                return i;
            }
        }
        return -1;
    }

    /**
     * index表示使用哪一种置换算法
     */
    public Opcode[] displace(int index) {
        Algorithm al = new Algorithm();
        switch (index) {
            case 0:
            case 1:
                return al.replace1();
            case 2:
                return al.replace2();
            case 3:
                return al.replace3();
        }
        return null;

    }
    /*

     */


    public short[] processing() {


        /**
         * 新指令数组
         */
        ArrayList<Short> newIns = new ArrayList<>();
        for (int i = 0; i < ins.length; i++) {
            newIns.add(ins[i]);
        }
        int MAXLENGTH = ins.length;//作为分支循环语句的结束条件
        /**
         * 记录新旧指令之间的差值
         */
        int offset = 0;
        int curOpcode = -1;

        int protectnumber = mOpt.whichSDK;
//        Opcode[] allOpcode= displace(3);
        logger.info("选择保护方案:" + protectnumber);
        Opcode[] allOpcode = displace(3);


        InstHelper instHelper = new InstHelper(mOpt);
        int pos = 0; //用来确定当前cur位置的
        do {
//            if(pos < FinishPos) {
            curOpcode = ins[pos] & 0xff;
//            }
            switch (allOpcode[curOpcode].name()) {
                case "OP_UNUSED_79":
                case "OP_UNUSED_7A":
                case "OP_UNUSED_73":

                    logger.info(allOpcode[curOpcode].name);
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "NOP":
                    logger.info(allOpcode[curOpcode].name);
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE":
                    logger.info(allOpcode[curOpcode].name + INST_A(ins, pos) + INST_B(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_FROM16":
                    logger.info(allOpcode[curOpcode].name + INST_AA(ins, pos) + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_16":
                    logger.info(allOpcode[curOpcode].name + FETCH(ins, pos) + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_WIDE":
                    logger.info(allOpcode[curOpcode].name + INST_A(ins, pos) + INST_B(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_WIDE_FROM16":
                    logger.info(allOpcode[curOpcode].name + INST_AA(ins, pos) + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_WIDE_16":
                    logger.info(allOpcode[curOpcode].name + FETCH(ins, pos) + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_OBJECT":
                    logger.info(allOpcode[curOpcode].name + INST_A(ins, pos) + INST_B(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_OBJECT_FROM16":
                    logger.info(allOpcode[curOpcode].name + INST_AA(ins, pos) + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_OBJECT_16":
                    logger.info(allOpcode[curOpcode].name + FETCH(ins, pos) + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "MOVE_RESULT":
                case "MOVE_RESULT_WIDE":
                case "MOVE_RESULT_OBJECT":
                case "MOVE_EXCEPTION":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;

                case "RETURN_VOID":
                    logger.info(allOpcode[curOpcode].name);
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "RETURN":
                case "RETURN_WIDE":
                case "RETURN_OBJECT":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "CONST_4":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " #0x" + ((INST_A(ins, pos) << 28) >> 28));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;

                case "CONST_16":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " #0x" + ((FETCH(ins, pos) << 16) >> 16));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "CONST":
                    logger.info(allOpcode[curOpcode].name + " " + INST_AA(ins, pos) + " #0x" + (FETCHWIDE(ins, pos)));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "CONST_HIGH16":
                    logger.info(allOpcode[curOpcode].name + " " + INST_AA(ins, pos) + " #0x" + ((FETCH(ins, pos) << 16)));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "CONST_WIDE_16":
                    logger.info(allOpcode[curOpcode].name + " " + INST_AA(ins, pos) + " #0x" + (((FETCH(ins, pos) << 48) >> 48)));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "CONST_WIDE_32":
                case "CONST_WIDE":
                    logger.info(allOpcode[curOpcode].name + " " + INST_AA(ins, pos) + " #0x" + ((FETCH64(ins, pos))));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "CONST_WIDE_HIGH16":
                    logger.info(allOpcode[curOpcode].name + " " + INST_AA(ins, pos) + " #0x" + (FETCHWIDE(ins, pos)));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;

                case "CONST_STRING":
                    logger.info(allOpcode[curOpcode].name + " v" + (ins[pos] >> 8) + " String@" + short2FormatString(ins[pos + 1]));
//                    if (findExistString(instHelper.getStringItem(ins[pos + 1] - 1)) == -1) {
                    mstringDatas.add(new DexFileFormat.StringData());
                    mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getStringItem(ins[pos + 1]);
//                    } else {
//                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getStringItem(findExistString(instHelper.getStringItem(ins[pos + 1] - 1)));
//                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mstringDatas.size() - 1));//在当前指令尾部加上相关字段索引偏移
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "CONST_STRING_JUMBO":
                    logger.info(allOpcode[curOpcode].name + " v" + (ins64[pos] >> 8) + " String@" + short2FormatString(ins64[pos + 1]));
                    if (findExistString(instHelper.getStringItem(ins[pos + 1])) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getStringItem(ins[pos + 1]);
                    } else {
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getStringItem(findExistString(instHelper.getStringItem(ins[pos + 1])));
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset - 1, (short) (mstringDatas.size() - 1));//在当前指令尾部加上相关字段索引偏移
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "MONITOR_ENTER":
                case "MONITOR_EXIT":
                    logger.info(allOpcode[curOpcode].name);
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;

                case "CONST_CLASS":
                case "CHECK_CAST":
                    int t = ins[pos + 1];
                    logger.info(allOpcode[curOpcode].name + " v" + INST_A(ins, pos) + " v" + INST_B(ins, pos) + " type@" + short2FormatString(ins[pos + 1]));
                    mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                    if (findExistString(instHelper.getTypeItem(ins[pos + 1])) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(ins[pos + 1]);
                        //这块需要用getNonMethodTypeItem，不需要去掉L和;
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;
                    } else {
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(instHelper.getTypeItem(ins[pos + 1]));
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mTypeItemDates.size() - 1));
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "INSTANCE_OF":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_A(ins, pos) + " v" + INST_B(ins, pos) + " type@" + short2FormatString(ins[pos + 1]));
                    mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                    if (findExistString(instHelper.getTypeItem(ins[pos + 1])) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getTypeItem(ins[pos + 1]);

                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;
                    } else {
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(instHelper.getTypeItem(ins[pos + 1]));
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mTypeItemDates.size() - 1));
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "ARRAY_LENGTH":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " v" + INST_A(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "NEW_INSTANCE":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " " + FETCH(ins, pos));
                    //此时判定条件为没有相同的字符串f。
                    mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                    if (findExistString(instHelper.getTypeItem(ins[pos + 1])) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getTypeItem(ins[pos + 1]);
                        //对内部类进行判断，如果存在内部类的话直接跳出。
                        for(int i = 0;i<mstringDatas.get(mstringDatas.size() - 1).stringItems.str.length;i++){
                            if(mstringDatas.get(mstringDatas.size() - 1).stringItems.str[i]=='$'){
                                logger.info("待保护方法中存在内部类，目前保护方案无法进行对其进行保护，因此造成的不便我们深感抱歉。后续将不断完善功能，谢谢！");
                                System.exit(0);
                            }
                        }
//                        logger.info(mstringDatas.get(mstringDatas.size() - 1).stringItems.str);
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;
                    } else {
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(instHelper.getTypeItem(ins[pos + 1]));
                    }
                    //获取字符串数据的类型及字符串。
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mTypeItemDates.size() - 1));
                    //对new-instance替换引用信息
                    mInstance.add(new Instance());
                    mInstance.get(mInstance.size() - 1).pos = offset + pos;

                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;

                case "NEW_ARRAY":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " v" + INST_A(ins, pos) + " type@" + short2FormatString(FETCH(ins, pos)));
//                    if (findExistString(instHelper.getNonMethodTypeItem(ins[pos + 1])) == -1) {
                    mstringDatas.add(new DexFileFormat.StringData());
                    mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(ins[pos + 1]);
//                    } else {
//                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(findExistString(instHelper.getNonMethodTypeItem(ins[pos + 1])));
//                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mstringDatas.size() - 1));//在当前指令尾部加上相关字段索引偏移
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }

                    break;
                case "FILLED_NEW_ARRAY":
                    logger.info(allOpcode[curOpcode].name + getInvokePara(ins, pos) + "type@" + FETCH(ins, pos));
                    mstringDatas.add(new DexFileFormat.StringData());
                    mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(ins[pos + 1]);
//                    } else {
//                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(findExistString(instHelper.getNonMethodTypeItem(ins[pos + 1])));
//                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mstringDatas.size() - 1));//在当前指令尾部加上相关字段索引偏移
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;

                case "FILLED_NEW_ARRAY_RANGE":
                    logger.info(allOpcode[curOpcode].name + getInvokeRange(ins, pos) + "type@" + FETCH(ins, pos));
                    mstringDatas.add(new DexFileFormat.StringData());
                    mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(ins[pos + 1]);
//                    } else {
//                        mstringDatas.get(mstringDatas.size() - 1).stringItems = instHelper.getNonMethodTypeItem(findExistString(instHelper.getNonMethodTypeItem(ins[pos + 1])));
//                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    //在新指令相应位置追加信息，并且计算新旧偏移
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mstringDatas.size() - 1));//在当前指令尾部加上相关字段索引偏移
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "FILL_ARRAY_DATA":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + ":array_" + INST_A(ins, pos) + "offset@" + FETCH(ins, pos));
                    //此处开始记录插入位置
                    mArrayDate.add(new ArrayDate());
                    mArrayDate.get(mArrayDate.size() - 1).pos = pos + offset + 1;//始终记录新数组的位置
                    mArrayDate.get(mArrayDate.size() - 1).keyvalue = FETCH(ins, pos);
                    mArrayDate.get(mArrayDate.size() - 1).oldpos = pos;
                    //FinishPos = mArrayDate.get(mArrayDate.size()-1).pos;
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));

                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;

                    break;
                case "THROW":
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "GOTO":
                    logger.info(allOpcode[curOpcode].name + " :goto_0");
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "GOTO_16":
                    logger.info(allOpcode[curOpcode].name + " + " + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "GOTO_32":
                    logger.info(allOpcode[curOpcode].name + " + " + FETCHWIDE(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "PACKED_SWITCH":
                case "SPARSE_SWITCH":
                    logger.info(allOpcode[curOpcode].name + INST_AA(ins, pos) + " +" + FETCHWIDE(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                /***
                 * function:比较指令。cmpkind(vAA,vBB,vCC);对于寄存器的值进行比较。
                 */
                case "CMPL_FLOAT":
                case "CMPG_FLOAT":
                case "CMPL_DOUBLE":
                case "CMPG_DOUBLE":
                case "CMP_LONG":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " v" + FETCHINSTA(ins, pos) + " v" + FETCHINSTB(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                /***
                 * functino:if-test条件跳转指令。if-testz条件跳转指令
                 */
                case "IF_EQ":
                case "IF_NE":
                case "IF_LT":
                case "IF_GE":
                case "IF_GT":
                case "IF_LE":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " v" + INST_A(ins, pos) + " :cond_0");
//                    int skip = FETCH(ins,pos);
//                    pos = pos + skip;
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "IF_EQZ":
                case "IF_NEZ":
                case "IF_LTZ":
                case "IF_GEZ":
                case "IF_GTZ":
                case "IF_LEZ":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " :" + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                /***
                 * function:字段操作指令。即用来对对象实例的字段进入读写操作。
                 */
                case "AGET":
                case "AGET_WIDE":
                case "AGET_OBJECT":
                case "AGET_BOOLEAN":
                case "AGET_BYTE":
                case "AGET_CHAR":
                case "AGET_SHORT":
                case "APUT":
                case "APUT_WIDE":
                case "APUT_OBJECT":
                case "APUT_BOOLEAN":
                case "APUT_BYTE":
                case "APUT_CHAR":
                case "APUT_SHORT":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " v" + FETCHINSTA(ins, pos) + " v" + FETCHINSTB(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "IGET":
                case "IGET_WIDE":
                case "IGET_OBJECT":
                case "IGET_BOOLEAN":
                case "IGET_BYTE":
                case "IGET_CHAR":
                case "IGET_SHORT":
                case "IPUT":
                case "IPUT_WIDE":
                case "IPUT_OBJECT":
                case "IPUT_BOOLEAN":
                case "IPUT_BYTE":
                case "IPUT_CHAR":
                case "IPUT_SHORT":
                    //判断pv寄存器。
                    int nflag2 = this.separatorData.registerSize - this.separatorData.paramSize - 1;
                    if (INST_A(ins, pos) < nflag2) {
                        logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " v" + INST_A(ins, pos) + " " + FETCH(ins, pos));
                    } else {
                        logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " p" + (INST_A(ins, pos) - nflag2) + " " + FETCH(ins, pos));
                    }

                    GetField getField = instHelper.getFiledItem(FETCH(ins, pos));
                    mfieldItemDates.add(new DexFileFormat.FieldItemDate());//建立field data数据结构

                    //判断className有没有重复
                    if (findExistString(getField.className) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = getField.className;
                        //填写mfielfItem的classNameIdx
                        mfieldItemDates.get(mfieldItemDates.size() - 1).classNameIdx = mstringDatas.size() - 1;
                        //写入class_name
                    } else {
                        mfieldItemDates.get(mfieldItemDates.size() - 1).classNameIdx = findExistString(getField.className);
                    }
                    //判断typeName有没有重复
                    if (findExistString(getField.typeName) == -1) {
                        //写入typename
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = getField.typeName;

                        //写入mfielfItem的typeIdx
                        mfieldItemDates.get(mfieldItemDates.size() - 1).typeIdx = mstringDatas.size() - 1;
                    } else {
                        mfieldItemDates.get(mfieldItemDates.size() - 1).typeIdx = findExistString(getField.typeName);
                    }
                    //判断field有没有重复
                    if (findExistString(getField.filed) == -1) {
                        //写入filed
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = getField.filed;
                        //写入mfieldItem的fieldNameIdx
                        mfieldItemDates.get(mfieldItemDates.size() - 1).fieldNameIdx = mstringDatas.size() - 1;
                    } else {
                        mfieldItemDates.get(mfieldItemDates.size() - 1).fieldNameIdx = findExistString(getField.filed);
                    }


                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    //将filed的索引写入新数组
                    newIns.add(pos + offset, (short) (mfieldItemDates.size() - 1));
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "SGET":
                case "SGET_WIDE":
                case "SGET_OBJECT":
                case "SGET_BOOLEAN":
                case "SGET_BYTE":
                case "SGET_CHAR":
                case "SGET_SHORT":
                case "SPUT":
                case "SPUT_WIDE":
                case "SPUT_OBJECT":
                case "SPUT_BOOLEAN":
                case "SPUT_BYTE":
                case "SPUT_CHAR":
                case "SPUT_SHORT":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " " + FETCH(ins, pos));
                    GetField getField1 = instHelper.getFiledItem(FETCH(ins, pos));
                    mfieldItemDates.add(new DexFileFormat.FieldItemDate());
                    if (findExistString(getField1.className) == -1) {
                        //写入class_name
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = getField1.className;
                        //填写mfielfItem的classNameIdx
                        mfieldItemDates.get(mfieldItemDates.size() - 1).classNameIdx = mstringDatas.size() - 1;
                    } else {
                        mfieldItemDates.get(mfieldItemDates.size() - 1).classNameIdx = findExistString(getField1.className);
                    }
                    if (findExistString(getField1.typeName) == -1) {
                        //写入typename
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = getField1.typeName;
                        //写入mfielfItem的typeIdx
                        mfieldItemDates.get(mfieldItemDates.size() - 1).typeIdx = mstringDatas.size() - 1;
                    } else {
                        mfieldItemDates.get(mfieldItemDates.size() - 1).typeIdx = findExistString(getField1.typeName);
                    }
                    if (findExistString(getField1.filed) == -1) {
                        //写入filed
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = getField1.filed;
                        //写入mfieldItem的fieldNameIdx
                        mfieldItemDates.get(mfieldItemDates.size() - 1).fieldNameIdx = mstringDatas.size() - 1;
                    } else {
                        mfieldItemDates.get(mfieldItemDates.size() - 1).fieldNameIdx = findExistString(getField1.filed);
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    //将filed的索引写入新数组
                    newIns.add(pos + offset, (short) (mfieldItemDates.size() - 1));
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                //test.add(3)，调用3的乘法。
                case "INVOKE_DIRECT":
                    logger.info(allOpcode[curOpcode].name + getInvokePara(ins, pos) + "method@" + FETCH(ins, pos));
                    Method oldMethod1 = instHelper.getMethod(FETCH(ins, pos));
                    mMethodItemDates.add(new DexFileFormat.MethodItemDate());//首先生成一个用来存储当前method的结构
                    if (findExistString(oldMethod1.className) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod1.className;

                        //为相应的TypeItem索引填值
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;

                        //为当前method的classnameid索引填值,其索引指向TypeItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
                    } else {
//                        if (findExistTypeItem(findExistString(oldMethod1.className)) == -1) {
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(oldMethod1.className);
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
//                        } else {
//                            mMethodItemDates.get(mTypeItemDates.size() - 1).classNameIdx = findExistTypeItem(findExistString(oldMethod1.className));
//                        }
                    }
                    if (findExistString(oldMethod1.proto) == -1) {
                        //将proto写入StringItem
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod1.proto;
                        //为当前method的proto索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = findExistString(oldMethod1.proto);
                    }
                    if (findExistString(oldMethod1.methodName) == -1) {
                        //将methodName写入StringItem中
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod1.methodName;
                        //为当前method的methodName索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = findExistString(oldMethod1.methodName);
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));

                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mMethodItemDates.size() - 1));
                    //新建值
                    mInstance.get(mInstance.size() - 1).val = mMethodItemDates.size() - 1;
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "INVOKE_VIRTUAL":
                case "INVOKE_SUPER":
                case "INVOKE_STATIC":
                case "INVOKE_INTERFACE":
                    logger.info(allOpcode[curOpcode].name + getInvokePara(ins, pos) + "method@" + FETCH(ins, pos));
                    Method oldMethod2 = instHelper.getMethod(FETCH(ins, pos));
                    mMethodItemDates.add(new DexFileFormat.MethodItemDate());//首先生成一个用来存储当前method的结构
                    if (findExistString(oldMethod2.className) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod2.className;

                        //为相应的TypeItem索引填值
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;

                        //为当前method的classnameid索引填值,其索引指向TypeItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
                    } else {
//                        if (findExistTypeItem(findExistString(oldMethod1.className)) == -1) {
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(oldMethod2.className);
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
//                        } else {
//                            mMethodItemDates.get(mTypeItemDates.size() - 1).classNameIdx = findExistTypeItem(findExistString(oldMethod1.className));
//                        }
                    }
                    if (findExistString(oldMethod2.proto) == -1) {
                        //将proto写入StringItem
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod2.proto;
                        //为当前method的proto索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = findExistString(oldMethod2.proto);
                    }
                    if (findExistString(oldMethod2.methodName) == -1) {
                        //将methodName写入StringItem中
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod2.methodName;
                        //为当前method的methodName索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = findExistString(oldMethod2.methodName);
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));

                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mMethodItemDates.size() - 1));

                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;


//                case "INVOKE_STATIC":
//                    logger.info(allOpcode[curOpcode].name + FETCH(ins, pos));
//                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
//                    break;
//                case "INVOKE_INTERFACE":
//                    logger.info(allOpcode[curOpcode].name + INST_AA(ins, pos));
//                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
//                    break;
                case "INVOKE_DIRECT_RANGE":
                    logger.info(allOpcode[curOpcode].name + getInvokeRange(ins, pos) + "method@" + FETCH(ins, pos));
                    Method oldMethod = instHelper.getMethod(FETCH(ins, pos));
                    mMethodItemDates.add(new DexFileFormat.MethodItemDate());//首先生成一个用来存储当前method的结构
                    if (findExistString(oldMethod.className) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod.className;

                        //为相应的TypeItem索引填值
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;

                        //为当前method的classnameid索引填值,其索引指向TypeItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
                    } else {
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(oldMethod.className);
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
                    }
                    if (findExistString(oldMethod.proto) == -1) {
                        //将proto写入StringItem
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod.proto;
                        //为当前method的proto索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = findExistString(oldMethod.proto);
                    }
                    if (findExistString(oldMethod.methodName) == -1) {
                        //将methodName写入StringItem中
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod.methodName;
                        //为当前method的methodName索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = findExistString(oldMethod.methodName);
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mMethodItemDates.size() - 1));
                    //新建值,新加的，不确定成不成。
                    mInstance.get(mInstance.size() - 1).val = mMethodItemDates.size() - 1;
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "INVOKE_VIRTUAL_RANGE":
                case "INVOKE_SUPER_RANGE":

                case "INVOKE_STATIC_RANGE":
                case "INVOKE_INTERFACE_RANGE":
                    logger.info(allOpcode[curOpcode].name + getInvokeRange(ins, pos) + "method@" + FETCH(ins, pos));
                    Method oldMethod3 = instHelper.getMethod(FETCH(ins, pos));
                    mMethodItemDates.add(new DexFileFormat.MethodItemDate());//首先生成一个用来存储当前method的结构
                    if (findExistString(oldMethod3.className) == -1) {
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod3.className;

                        //为相应的TypeItem索引填值
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = mstringDatas.size() - 1;

                        //为当前method的classnameid索引填值,其索引指向TypeItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
                    } else {
                        mTypeItemDates.add(new DexFileFormat.TypeItemDate());
                        mTypeItemDates.get(mTypeItemDates.size() - 1).typeItem = findExistString(oldMethod3.className);
                        mMethodItemDates.get(mMethodItemDates.size() - 1).classNameIdx = mTypeItemDates.size() - 1;
                    }
                    if (findExistString(oldMethod3.proto) == -1) {
                        //将proto写入StringItem
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod3.proto;
                        //为当前method的proto索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).protoIdx = findExistString(oldMethod3.proto);
                    }
                    if (findExistString(oldMethod3.methodName) == -1) {
                        //将methodName写入StringItem中
                        mstringDatas.add(new DexFileFormat.StringData());
                        mstringDatas.get(mstringDatas.size() - 1).stringItems = oldMethod3.methodName;
                        //为当前method的methodName索引填值，其指向StringItem
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = mstringDatas.size() - 1;
                    } else {
                        mMethodItemDates.get(mMethodItemDates.size() - 1).methodNameIdx = findExistString(oldMethod3.methodName);
                    }
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    newIns.add(pos + offset, (short) (mMethodItemDates.size() - 1));
                    offset += OFFSETLENGTH;
                    for (ArrayDate arrayDate : mArrayDate) {
                        arrayDate.off += OFFSETLENGTH;
                    }
                    break;
                case "NEG_INT":
                case "NOT_INT":
                case "NEG_LONG":
                case "NOT_LONG":
                case "NEG_FLOAT":
                case "NEG_DOUBLE":
                case "INT_TO_LONG":
                case "INT_TO_FLOAT":
                case "INT_TO_DOUBLE":
                case "LONG_TO_INT":
                case "LONG_TO_FLOAT":
                case "LONG_TO_DOUBLE":
                case "FLOAT_TO_INT":
                case "FLOAT_TO_LONG":
                case "FLOAT_TO_DOUBLE":
                case "DOUBLE_TO_INT":
                case "DOUBLE_TO_LONG":
                case "DOUBLE_TO_FLOAT":
                case "INT_TO_BYTE":
                case "INT_TO_CHAR":
                case "INT_TO_SHORT":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " v" + INST_A(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "ADD_INT":
                case "SUB_INT":
                case "MUL_INT":
                case "DIV_INT":
                case "REM_INT":
                case "AND_INT":
                case "OR_INT":
                case "XOR_INT":
                case "SHL_INT":
                case "SHR_INT":
                case "USHR_INT":
                case "ADD_LONG":
                case "SUB_LONG":
                case "MUL_LONG":
                case "DIV_LONG":
                case "REM_LONG":
                case "AND_LONG":
                case "OR_LONG":
                case "XOR_LONG":
                case "SHL_LONG":
                case "SHR_LONG":
                case "USHR_LONG":
                case "ADD_FLOAT":
                case "SUB_FLOAT":
                case "MUL_FLOAT":
                case "DIV_FLOAT":
                case "REM_FLOAT":
                case "ADD_DOUBLE":
                case "SUB_DOUBLE":
                case "MUL_DOUBLE":
                case "DIV_DOUBLE":
                case "REM_DOUBLE":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " v" + FETCHINSTA(ins, pos) + "v" + FETCHINSTB(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "ADD_INT_2ADDR":
                case "SUB_INT_2ADDR":
                case "MUL_INT_2ADDR":
                case "DIV_INT_2ADDR":
                case "REM_INT_2ADDR":
                case "AND_INT_2ADDR":
                case "OR_INT_2ADDR":
                case "XOR_INT_2ADDR":
                case "SHL_INT_2ADDR":
                case "SHR_INT_2ADDR":
                case "USHR_INT_2ADDR":
                case "ADD_LONG_2ADDR":
                case "SUB_LONG_2ADDR":
                case "MUL_LONG_2ADDR":
                case "DIV_LONG_2ADDR":
                case "REM_LONG_2ADDR":
                case "AND_LONG_2ADDR":
                case "OR_LONG_2ADDR":
                case "XOR_LONG_2ADDR":
                case "SHL_LONG_2ADDR":
                case "SHR_LONG_2ADDR":
                case "USHR_LONG_2ADDR":
                case "ADD_FLOAT_2ADDR":
                case "SUB_FLOAT_2ADDR":
                case "MUL_FLOAT_2ADDR":
                case "DIV_FLOAT_2ADDR":
                case "REM_FLOAT_2ADDR":
                case "ADD_DOUBLE_2ADDR":
                case "SUB_DOUBLE_2ADDR":
                case "MUL_DOUBLE_2ADDR":
                case "DIV_DOUBLE_2ADDR":
                case "REM_DOUBLE_2ADDR":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_B(ins, pos) + " v" + INST_A(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "ADD_INT_LIT16":
                case "RSUB_INT":
                case "MUL_INT_LIT16":
                case "DIV_INT_LIT16":
                case "REM_INT_LIT16":
                case "AND_INT_LIT16":
                case "OR_INT_LIT16":
                case "XOR_INT_LIT16":
                case "IGET_VOLATILE":
                case "IPUT_VOLATILE":
                case "SGET_VOLATILE":
                case "SPUT_VOLATILE":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_A(ins, pos) + " +v" + INST_B(ins, pos) + " #int " + FETCH(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "ADD_INT_LIT8":
                case "RSUB_INT_LIT8":
                case "MUL_INT_LIT8":
                case "DIV_INT_LIT8":
                case "REM_INT_LIT8":
                case "AND_INT_LIT8":
                case "OR_INT_LIT8":
                case "XOR_INT_LIT8":
                case "SHL_INT_LIT8":
                case "SHR_INT_LIT8":
                case "USHR_INT_LIT8":
                    logger.info(allOpcode[curOpcode].name + " v" + INST_AA(ins, pos) + " v" + FETCHINSTA(ins, pos) + " #int" + FETCHINSTB(ins, pos));
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
                case "IGET_OBJECT_VOLATILE":
                case "IGET_WIDE_VOLATILE":
                case "IPUT_WIDE_VOLATILE":
                case "SGET_WIDE_VOLATILE":
                case "SPUT_WIDE_VOLATILE":
                case "THROW_VERIFICATION_ERROR":
                case "EXECUTE_INLINE":
                case "EXECUTE_INLINE_RANGE":
                case "INVOKE_OBJECT_INIT_RANGE":
                case "RETURN_VOID_BARRIER":
                case "IGET_QUICK":
                case "IGET_WIDE_QUICK":
                case "IGET_OBJECT_QUICK":
                case "IPUT_QUICK":
                case "IPUT_WIDE_QUICK":
                case "IPUT_OBJECT_QUICK":
                case "INVOKE_VIRTUAL_QUICK":
                case "INVOKE_VIRTUAL_QUICK_RANGE":
                case "INVOKE_SUPER_QUICK":
                case "INVOKE_SUPER_QUICK_RANGE":
                case "IPUT_OBJECT_VOLATILE":
                case "SGET_OBJECT_VOLATILE":
                case "SPUT_OBJECT_VOLATILE":
                case "OP_UNUSED_FF":
                    newIns.set(offset + pos, (short) ((ins[pos] >> 8 << 8) | allOpcode[curOpcode].value));
                    pos = pos + allOpcode[curOpcode].jumboOpcodeValue;
                    break;
            }
        } while (pos < MAXLENGTH && pos < getEndofIns());
        //返回之前对fill-array-data指令中的偏析重新填值
        for (int i = 0; i < mArrayDate.size(); i++) {
            short oldArrayOff = 0;
            short newArrayOff;

            if (mArrayDate.get(i).size == 1) {
                oldArrayOff = newIns.get(mArrayDate.get(i).pos);
                newArrayOff = (short) (oldArrayOff + mArrayDate.get(i).off);
                //将修改填入新数组
                newIns.set(mArrayDate.get(i).pos, newArrayOff);

            } else
                logger.info("其他");
        }
        for (int i = 0; i < mInstance.size(); i++) {
            newIns.set(mInstance.get(i).pos, (short) mInstance.get(i).val);
        }


        //返回
        int newInsSize = newIns.size();
        short[] result = new short[newInsSize];
        String[] result1 = new String[newInsSize];
        for (int i = 0; i < newInsSize; i++) {
            result[i] = newIns.get(i);
            result1[i] = Integer.toHexString(result[i]);
            System.out.printf(result1[i] + " ");
        }

        return result;
    }

    /**
     * 作用：动态跟踪marraydate中的相关数据，定位指令的结束为止
     *
     * @return 返回数组的开始位置
     */
    public int getEndofIns() {

        int min = Integer.MAX_VALUE;
        for (ArrayDate arrayDate : mArrayDate) {
            int curvalue = arrayDate.keyvalue + arrayDate.oldpos;
            min = min < curvalue ? min : curvalue;
        }
        return min;
    }


}