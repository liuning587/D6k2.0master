package com.controller.UserManage;

import com.entity.user_Login;
import com.service.Login.LoginHandle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by zyj on 2017/7/12.
 */
@RestController
@RequestMapping(value = "/userLogin")
public class LoginController {

    @Autowired
    LoginHandle loginHandle;

    @RequestMapping(value = "/Login", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<user_Login> getDevicesOfSubStation(HttpServletRequest request) {
        String strUsername = request.getParameter("username");
        String strPassword = request.getParameter("password");

        user_Login userLogin = loginHandle.login(strUsername, strPassword);

        return new ResponseEntity<user_Login>(userLogin, HttpStatus.OK);
    }
}
