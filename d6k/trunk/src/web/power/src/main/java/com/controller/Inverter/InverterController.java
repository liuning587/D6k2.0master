package com.controller.Inverter;

import com.entity.Inverter;
import com.entity.StationInfo.DeviceInfoOfReport;
import com.entity.StationInfo.InverterInfo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * Created by zyj on 2017/6/29.
 */
@RestController
@RequestMapping(value = "/stationInfo")
public class InverterController {

    @Autowired
    Inverter inverter;

    @RequestMapping(value = "/{stationId}/inverter", method = RequestMethod.GET)
    @ResponseBody
    public ResponseEntity<InverterInfo> getDevicesOfSubStation(@PathVariable("stationId") Integer id) {
        InverterInfo info = new InverterInfo();

        // 获取逆变器测量模板
        String strInverterElem = "";
        for (int i = 0; i < 12; i++) {
            strInverterElem += inverter.createInverterWholeElem();
        }

        info.setStrData(strInverterElem);

        return new ResponseEntity<InverterInfo>(info, HttpStatus.OK);
    }
}
