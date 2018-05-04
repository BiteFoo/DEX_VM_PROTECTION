package lz.android.shell.controlcentre;



import org.apache.commons.cli.*;
import org.apache.log4j.*;

import java.io.*;
import java.util.Scanner;


/**
 * Created by buwai on 2015/4/1.
 */
public class EntryPoint {

    private final static Logger log = Logger.getLogger(EntryPoint.class);


    private static Options options;

    static {
        options = new Options();
        options.addOption("h", false, "使用帮助");
        options.addOption("s", true, "apk文件路径。");
        options.addOption("o", true, "输出目录");
    }

    public static void main(String[] args) {
        log.info("------ 进入控制中心 ------");
        try {
            BasicParser parser = new BasicParser();
            CommandLine cl = null;
            cl = parser.parse(options, args);

            if (cl.hasOption('h')) {
                usage();
            }

            ControlCentreOption opt = new ControlCentreOption();
            if (cl.hasOption('s')) {
                opt.apkFile = new File(cl.getOptionValue('s'));
            }
            if (null == opt.apkFile) {
                usage();
            }

            if (cl.hasOption('o')) {
                opt.outDir = new File(cl.getOptionValue('o'));
            }
            if (null == opt.outDir) {
                usage();
            }

            ControlCentre controlCentre = new ControlCentre(opt);
            System.out.print("请输入您要保护的类名(比如:com.example.lz.advmptesteasy.MainActivity):");
            Scanner scanner01 = new Scanner(System.in);
            String InputProtectClassName = null;
            InputProtectClassName = scanner01.next();//输入待保护类名
            System.out.print("请输入您要保护的方法名(比如:add):");
            Scanner scanner02 = new Scanner(System.in);//输入待保护方法名
            String ProtectMethodName = null;
            ProtectMethodName = scanner02.next();
            log.info("------ 进入控制中心 ------");
            log.info("开始加固。");
            String ProtectClassName=null;
            ProtectClassName=DealClassName(InputProtectClassName);//对类名进行处理
//            Utils.copyFile(opt.apkFile.getAbsolutePath(),opt.outDir.getAbsolutePath()+File.separator+"apk_shelled.apk");
//            log.info("加固完成");
            if (controlCentre.shell(ProtectClassName,ProtectMethodName,InputProtectClassName)) {
                log.info("加固完成");
            }

        } catch (ParseException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        log.info("------ 离开控制中心 ------");
    }

    private static void usage() {
        HelpFormatter help = new HelpFormatter();
        help.printHelp("-s <dex文件路径> -o <输出目录> [-c <配置文件路径>]", options);
        System.exit(-1);
    }
    public static String DealClassName(String ProtectClassName){
        ProtectClassName = ProtectClassName.replace(".","/");
        ProtectClassName = "L"+ProtectClassName+";";
        return ProtectClassName;
    }
}
