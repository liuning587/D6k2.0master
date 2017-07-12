package com.service.Login;

import com.entity.user_Login;
import com.entity.user_info;
import com.service.UserInfo.ModifyPwdHandle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by zyj on 2017/7/12.
 */
@Service("LoginHandle")
public class LoginHandle {
    @Autowired
    user_Login userLogin;

    @Autowired
    ModifyPwdHandle modifyPwdHandle;

    public user_Login login(String strUsername, String strPassword) {
        if (strUsername.isEmpty() || strPassword.isEmpty()) {
            userLogin.setnStatus(-1);
            return userLogin;
        }

        user_info userInfo = modifyPwdHandle.SelectUserInfoByUsername(strUsername);
        if (userInfo == null) {
            userLogin.setnStatus(-1);
            return userLogin;
        }

        if (!strPassword.equals(userInfo.getUserPwd())) {
            userLogin.setnStatus(-2);
            return userLogin;
        }

        userLogin.setnStatus(0);
        userLogin.setUserInfo(userInfo);

        return userLogin;
    }
}
