package com.controller.UserManage;

import com.service.UserInfo.ModifyPwdHandle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by zyj on 2017/7/10.
 */
@RestController
@RequestMapping(value = "/userManage")
public class UserManageController {

    @Autowired
    ModifyPwdHandle modifyPwdHandle;

    @RequestMapping(value = "/modifyPWD", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<Integer> getDevicesOfSubStation(HttpServletRequest request) {

        String strUsername = request.getParameter("username");
        String strOldPwd = request.getParameter("old_pwd");
        String strNewPwd = request.getParameter("new_pwd");

        Integer res = modifyPwdHandle.ModifyPasswordMothed(strUsername, strOldPwd, strNewPwd);

        return new ResponseEntity<Integer>(res, HttpStatus.OK);
    }
}
