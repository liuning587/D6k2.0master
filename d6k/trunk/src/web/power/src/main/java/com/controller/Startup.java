package com.controller;

import org.springframework.web.bind.annotation.RequestMapping;

/**
 * Created by zyj on 2017/6/20.
 */
public class Startup {
    @RequestMapping("/")
    public String index(){
        return "/index";
    }
}
