package com.entity;

import org.springframework.stereotype.Service;

/**
 * Created by zyj on 2017/7/12.
 */
@Service("user_Login")
public class user_Login {
    // 0 :登录成功
    // -1:用户名错误
    // 其他：密码错误
    Integer nStatus;

    user_info userInfo;

    public Integer getnStatus() {
        return nStatus;
    }

    public void setnStatus(Integer nStatus) {
        this.nStatus = nStatus;
    }

    public user_info getUserInfo() {
        return userInfo;
    }

    public void setUserInfo(user_info userInfo) {
        this.userInfo = userInfo;
    }
}
