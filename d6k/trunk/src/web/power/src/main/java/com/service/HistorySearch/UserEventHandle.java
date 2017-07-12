package com.service.HistorySearch;

import com.dao.user_eventMapper;
import com.entity.HistorySearch.userEventAndPageInfo;
import com.entity.user_event;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * Created by zyj on 2017/7/5.
 */
@Service("UserEventHandle")
public class UserEventHandle {

    @Autowired
    private user_eventMapper userEventMapper;

    @Autowired
    UserEventService userEventTime;

    public userEventAndPageInfo GetUserEventByTime(String strBeginTime, String strEndTime, int nPageSize, int nPageIndex) throws ParseException {

        userEventAndPageInfo userEventAndPageInfo = new userEventAndPageInfo();

        if (nPageSize <= 0 || nPageIndex <= 0) {
            return  userEventAndPageInfo;
        }

        //String strTime1 = "2017-07-01 00:00:00";
        SimpleDateFormat format= new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Date dateBegin = format.parse(strBeginTime);

        //String strTime2 = "2017-07-02 00:00:00";
        Date dateEnd = format.parse(strEndTime);

        userEventTime.setBeginTime(dateBegin);

        userEventTime.setEndTime(dateEnd);

        userEventTime.setLimitBegin(nPageSize*(nPageIndex-1));

        userEventTime.setLimitEnd(nPageSize*(nPageIndex-1) + nPageSize);

        userEventAndPageInfo.setListUserEvent(userEventMapper.selectUserEventByTime(userEventTime));

        userEventAndPageInfo.setRecordCnt(userEventMapper.selectAllUserEventByTime(userEventTime));

        return userEventAndPageInfo;
    }
}
