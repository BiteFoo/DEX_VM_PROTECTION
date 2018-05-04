package lz.android.shell.separator.config;

import lz.android.shell.base.TypeDescription;
import lz.android.shell.base.helper.TypeDescriptionHelper;
import org.jf.dexlib2.AccessFlags;
import org.jf.dexlib2.iface.Method;

/**
 * Created by buwai on 2015/4/1.
 */
public class ConfigHelper {

    private Config mConfig;

    public ConfigHelper (Config config) {
        mConfig = config;
    }

    /**
     * 是否有效。
     * @param method
     * @return true：有效。false：无效。
     */
    public boolean isValid(Method method,String Classname,String MethodName) {
        TypeDescription methodTypeDesc = TypeDescriptionHelper.convertByMethod(method);

        // 不支持方法中有try...catch的。
//        if (false == mConfig.isSupportTryCatch) {
//          // System.out.println(method.getImplementation().getRegisterCount());
////            if (0 != method.getImplementation().getTryBlocks().size()) {
////                System.out.println("Unsupported type");
//             return false;
////            }
//        }

        // 判断方法是否实现。
        if (null == method.getImplementation()) {
            return false;
        }
        else {
            // 不支持方法中有try...catch的。
//            if (false == mConfig.isSupportTryCatch) {
//                if (0 != method.getImplementation().getTryBlocks().size()) {
//                    System.out.println("Unsupported type");
//                    return false;
//                }
//            }
        }

        // 判断是否是native方法。
        if (0 != (method.getAccessFlags() & AccessFlags.NATIVE.getValue())) {
            return false;
        }
//
//        // 不支持<clinit>
        if ("<clinit>".equals(methodTypeDesc.methodName)) {
            return false;
        }

        // 不支持<init>
        if ("<init>".equals(methodTypeDesc.methodName)) {
            return false;
        }

        //对类名进行判断。
        if(!Classname.equals(method.getDefiningClass())){
            return false;
        }
//        Lcom/example/lz/advmptesteasy/MainActivity;
//        ddd1
        //对方法名进行判断。
        // TODO 这里现在是写死的！
        if ((!MethodName.equals(method.getName()))) {
            return false;
        }
        // 先判断黑名单。
        if (TypeDescriptionHelper.isMatchMethodInBlackList(mConfig.blackList, methodTypeDesc)) {
            return false;
        }

        // 再判断白名单。
        if (!TypeDescriptionHelper.isMatchMethodInWhiteList(mConfig.whiteList, methodTypeDesc)) {
            return false;
        }

        return true;
    }

}
