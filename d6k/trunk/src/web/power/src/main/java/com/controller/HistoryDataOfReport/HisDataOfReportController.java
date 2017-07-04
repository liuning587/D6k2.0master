package com.controller.HistoryDataOfReport;

import com.entity.StationInfo.DeviceInfoOfReport;
import com.entity.StationInfo.PointInfoOfReport;
import com.entity.StationInfo.RealtimeCurveInfo;
import com.entity.power_predict_4h;
import com.measurement.MeasurementValue;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by zyj on 2017/6/26.
 */
@RestController
@RequestMapping(value = "/historyData")
public class HisDataOfReportController {

    @RequestMapping(value = "/station/{id}/report", method = RequestMethod.GET)
    @ResponseBody
    public ResponseEntity<List<DeviceInfoOfReport>> getDevicesOfSubStation(@PathVariable("id") Integer id) {
        List<DeviceInfoOfReport> temp = new ArrayList<DeviceInfoOfReport>();
        DeviceInfoOfReport info1 = new DeviceInfoOfReport();
        info1.setStrName("设备1");
        info1.setnID(1);
        DeviceInfoOfReport info2 = new DeviceInfoOfReport();
        info2.setStrName("设备2");
        info2.setnID(2);
        DeviceInfoOfReport info3 = new DeviceInfoOfReport();
        info3.setStrName("设备3");
        info3.setnID(3);
        DeviceInfoOfReport info4 = new DeviceInfoOfReport();
        info4.setStrName("设备4");
        info4.setnID(4);
        DeviceInfoOfReport info5 = new DeviceInfoOfReport();
        info5.setStrName("设备5");
        info5.setnID(5);
        DeviceInfoOfReport info6 = new DeviceInfoOfReport();
        info6.setStrName("设备6");
        info6.setnID(6);

        temp.add(info1);
        temp.add(info2);
        temp.add(info3);
        temp.add(info4);
        temp.add(info5);
        temp.add(info6);

        return new ResponseEntity<List<DeviceInfoOfReport>>(temp, HttpStatus.OK);
    }

    @RequestMapping(value = "/station/{stationId}/{deviceId}/report", method = RequestMethod.GET)
    @ResponseBody
    public ResponseEntity<List<PointInfoOfReport>> getPointOfReport(@PathVariable("stationId") Integer nStationId, @PathVariable("deviceId") Integer nDeviceId) {
        List<PointInfoOfReport> temp = new ArrayList<PointInfoOfReport>();
        PointInfoOfReport info1 = new PointInfoOfReport();
        info1.setStrName("A相电流");
        info1.setnID(1);
        PointInfoOfReport info2 = new PointInfoOfReport();
        info2.setStrName("B相电流");
        info2.setnID(2);
        PointInfoOfReport info3 = new PointInfoOfReport();
        info3.setStrName("C相电流");
        info3.setnID(3);
        PointInfoOfReport info4 = new PointInfoOfReport();
        info4.setStrName("A相电压");
        info4.setnID(4);
        PointInfoOfReport info5 = new PointInfoOfReport();
        info5.setStrName("B相电压");
        info5.setnID(5);
        PointInfoOfReport info6 = new PointInfoOfReport();
        info6.setStrName("C相电压");
        info6.setnID(6);

        temp.add(info1);
        temp.add(info2);
        temp.add(info3);
        temp.add(info4);
        temp.add(info5);
        temp.add(info6);

        return new ResponseEntity<List<PointInfoOfReport>>(temp, HttpStatus.OK);
    }
}
