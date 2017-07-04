package com.controller.GetMenuInfo;

import com.measurement.MeasurementValue;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;


/**
 * Created by zyj on 2017/6/20.
 */
@RestController
@RequestMapping(value = "/menuInfo")
public class GetMenuInfoController {

    @RequestMapping(value = "/realTimeCurveMenu", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<MeasurementValue> getNormalValues() {

        MeasurementValue value = new MeasurementValue();
        value.setText("zhuyongjin");
        return new ResponseEntity<MeasurementValue>(value, HttpStatus.OK);
    }
}
