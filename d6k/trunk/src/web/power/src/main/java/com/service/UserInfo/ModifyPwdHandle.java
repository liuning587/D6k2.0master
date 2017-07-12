package com.service.UserInfo;

import com.dao.user_infoMapper;
import com.entity.user_info;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by zyj on 2017/7/11.
 */
@Service("ModifyPwdHandle")
public class ModifyPwdHandle {
    @Autowired
    private user_infoMapper userInfoMapper;

    // 修改密码
    // 返回0：修改成功
    // -1：密码错误
    // -2：用户名不存在
    // -3：密码修改失败
    public Integer ModifyPasswordMothed(String strUsername, String strOldPwd, String strNewPwd) {
        if (strUsername.isEmpty() || strOldPwd.isEmpty() || strNewPwd.isEmpty()) {
            return -3;
        }

        user_info userInfo = SelectUserInfoByUsername(strUsername);
        if (userInfo == null) {
            return -2;
        }

        if (!strOldPwd.equals(userInfo.getUserPwd())) {
            return -1;
        }

        userInfo.setUserPwd(strNewPwd);

        userInfoMapper.updateByPrimaryKey(userInfo);

        return 0;
    }

    // 通过用户名查找用户信息
    public user_info SelectUserInfoByUsername(String strUsername) {
        if (strUsername.isEmpty()) {
            return null;
        }

        user_info userInfo = userInfoMapper.selectByPrimaryKey(strUsername);
        if (userInfo == null) {
            return null;
        }

        return userInfo;
    }
}
