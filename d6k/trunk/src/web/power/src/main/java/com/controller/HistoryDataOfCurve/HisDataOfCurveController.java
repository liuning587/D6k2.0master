package com.controller.HistoryDataOfCurve;

import com.dao.power_predict_4hMapper;
import com.entity.power_predict_4h;
import com.measurement.MeasurementValue;
import com.measurement.RealTimeClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by zyj on 2017/6/16.
 */
@RestController
@RequestMapping(value = "/hisDataOfCurve")
public class HisDataOfCurveController {

    @Autowired
    RealTimeClient realTimeClient;

    @Autowired
    private power_predict_4hMapper mapper;

    @RequestMapping(value = "/shortCurve", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<MeasurementValue> getNormalValues(@RequestBody String info) throws ParseException {

        Integer id = 1;
        String strTime = "2013-01-24 00:00:00";
        power_predict_4h power = mapper.selectByPrimaryKey(id);

        SimpleDateFormat format= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Date date=format.parse(strTime);

        power_predict_4h power1 = mapper.selectByTime(date);

        MeasurementValue value = new MeasurementValue();
        value.setText("zhuyongjin");
        return new ResponseEntity<MeasurementValue>(value, HttpStatus.OK);
    }
}
