package com.controller.RealtimeCurve;

import com.entity.StationInfo.RealtimeCurveInfo;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zyj on 2017/6/23.
 */
@RestController
@RequestMapping(value = "/realtimeCurve")
public class RealtimeCurveController {
    /**
     * 获取一个厂站的所有设备
     * @param id 设备ID
     * @return 设备列表
     */
    @RequestMapping(value = "/station/{id}/dataType", method = RequestMethod.GET)
    public ResponseEntity<List<RealtimeCurveInfo>> getDevicesOfSubStation(@PathVariable("id") Integer id) {
        List<RealtimeCurveInfo> temp = new ArrayList<RealtimeCurveInfo>();
        RealtimeCurveInfo info1 = new RealtimeCurveInfo();
        info1.setStrName("有功功率");
        info1.setnID(1);
        RealtimeCurveInfo info2 = new RealtimeCurveInfo();
        info2.setStrName("风速");
        info2.setnID(2);
        RealtimeCurveInfo info3 = new RealtimeCurveInfo();
        info3.setStrName("风向");
        info3.setnID(3);
        RealtimeCurveInfo info4 = new RealtimeCurveInfo();
        info4.setStrName("温度");
        info4.setnID(4);
        RealtimeCurveInfo info5 = new RealtimeCurveInfo();
        info5.setStrName("湿度");
        info5.setnID(5);
        RealtimeCurveInfo info6 = new RealtimeCurveInfo();
        info6.setStrName("压力");
        info6.setnID(6);
        RealtimeCurveInfo info7 = new RealtimeCurveInfo();
        info7.setStrName("辐射度");
        info7.setnID(7);
        RealtimeCurveInfo info8 = new RealtimeCurveInfo();
        info8.setStrName("组件温度");
        info8.setnID(8);

        temp.add(info1);
        temp.add(info2);
        temp.add(info3);
        temp.add(info4);
        temp.add(info5);
        temp.add(info6);
        temp.add(info7);
        temp.add(info8);

        return new ResponseEntity<List<RealtimeCurveInfo>>(temp, HttpStatus.OK);
    }
}
