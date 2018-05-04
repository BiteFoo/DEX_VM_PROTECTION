package lz.android.shell.processins;



public  class Algorithm{
    public Opcode[] opCodes=Opcode.values();
        public  Opcode[] replace1() {
            System.out.println("第一组置换结果如下：");
            for(int i=0;i<opCodes.length;i++)
            {
                //System.out.println("置换之前 "+i+" "+ opCodes[i].name+" "+opCodes[i].value);
                opCodes[i].value= (short) ((opCodes[i].value+5)%256);//将opcode循环右移5位
               // System.out.println("置换之后 "+i+" "+ opCodes[i].name+" "+opCodes[i].value);
                System.out.println(opCodes[i].name()+" = 0x"+Integer.toHexString(opCodes[i].value)+",");
            }

            return opCodes;
        }
        //前后交换再循环右移77位
        public Opcode[] replace2() {
            System.out.println("第二组置换结果如下：");
            for(int i=0;i<opCodes.length/2;i++)
            {
//                System.out.println("置换之前 "+i+" "+ opCodes[i].name+" "+opCodes[i].value);
//                System.out.println(opCodes[i].name());
//                System.out.println("置换之前 "+String.valueOf(opCodes.length-1-i)+" "+ opCodes[opCodes.length-1-i].name+" "+opCodes[opCodes.length-i-1].value);
                short tmp=0;
                tmp=opCodes[i].value;
                opCodes[i].value=opCodes[opCodes.length-1-i].value;
                opCodes[opCodes.length-1-i].value=tmp;
                //右移77位
                opCodes[i].value= (short) ((opCodes[i].value+77)%256);
                opCodes[opCodes.length-1-i].value= (short) ((opCodes[opCodes.length-1-i].value+77)%256);
//                System.out.println("置换之后 "+i+" "+ opCodes[i].name()+" "+opCodes[i].value);
//                System.out.println("置换之后 "+(opCodes.length-1-i)+" "+ opCodes[opCodes.length-1-i].name()+" "+opCodes[opCodes.length-1-i].value);
//                for(int k=0;k<256;k++) {
//                    for (int j = 0; j < 256; j++) {
//                        if (opCodes[j].value == k){
//                            System.out.println(opCodes[j].name());
//                            break;
//                        }
//                    }
//                }
                System.out.println(opCodes[i].name()+" =  0x"+Integer.toHexString(opCodes[i].value)+",");
                System.out.println(opCodes[opCodes.length-1-i].name()+"  = 0x"+Integer.toHexString(opCodes[opCodes.length-1-i].value)+",");
            }
            return opCodes;
        }

       //不做处理
        public  Opcode[] replace3() {
            return opCodes;
        }



}
