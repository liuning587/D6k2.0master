package com.controller.HistorySearch;

import com.dao.user_eventMapper;
import com.entity.HistorySearch.userEventAndPageInfo;
import com.entity.user_event;
import com.measurement.MeasurementValue;
import com.service.HistorySearch.UserEventHandle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * Created by zyj on 2017/7/5.
 */
@RestController
@RequestMapping(value = "/historySearch")
public class HistorySearchController {

    @Autowired
    private UserEventHandle userEventHandle;
    //@Autowired
    //private user_eventMapper userEventMapper;

    @RequestMapping(value = "/userEvent", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<userEventAndPageInfo> getNormalValues(HttpServletRequest request) throws ParseException {

        String strBeginTime = request.getParameter("beginTime");
        String strEndTime = request.getParameter("endTime");
        String strPageSize = request.getParameter("pageSize");
        String strPageIndex = request.getParameter("pageIndex");

        int nPageSize = 0;
        int nPageIndex = 0;
        try {
            nPageSize = Integer.parseInt(strPageSize);
            nPageIndex = Integer.parseInt(strPageIndex);
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }

        if (nPageSize <= 0) {
            userEventAndPageInfo userEventAndPageInfo = new userEventAndPageInfo();
            return new ResponseEntity<userEventAndPageInfo>(userEventAndPageInfo, HttpStatus.OK);
        }

        userEventAndPageInfo userEventAndPageInfo = userEventHandle.GetUserEventByTime(strBeginTime, strEndTime, nPageSize, nPageIndex);

        return new ResponseEntity<userEventAndPageInfo>(userEventAndPageInfo, HttpStatus.OK);
        /*String strTime = "2017-07-01 00:00:00";
        SimpleDateFormat format= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Date dateBegin = format.parse(strTime);

        strTime = "2017-07-02 00:00:00";
        Date dateEnd = format.parse(strTime);

        List<user_event> listUserEvent = userEventMapper.selectUserEventByTime(dateBegin, dateEnd);


        return new ResponseEntity<List<user_event>>(listUserEvent, HttpStatus.OK);*/
    }
}

