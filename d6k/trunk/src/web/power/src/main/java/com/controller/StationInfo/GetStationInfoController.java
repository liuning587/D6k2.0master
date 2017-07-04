package com.controller.StationInfo;

import com.entity.StationInfo.StationInfo;
import com.measurement.MeasurementValue;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zyj on 2017/6/22.
 */
@RestController
public class GetStationInfoController {
    @RequestMapping(value = "/GetStationInfo", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<List<StationInfo>> getNormalValues() {

        List<StationInfo> temp = new ArrayList<StationInfo>();
        StationInfo info1 = new StationInfo();
        info1.setStrName("场站1");
        info1.setnStationID(1);
        StationInfo info2 = new StationInfo();
        info2.setStrName("场站2");
        info2.setnStationID(2);

        temp.add(info1);
        temp.add(info2);

        return new ResponseEntity<List<StationInfo>>(temp, HttpStatus.OK);
    }
}
