package com.service.HistorySearch;

import org.springframework.stereotype.Service;

import java.util.Date;

/**
 * Created by zyj on 2017/7/5.
 */
@Service("UserEventService")
public class UserEventService {
    Date beginTime;

    Date endTime;

    int limitBegin;

    int limitEnd;

    public int getLimitBegin() {
        return limitBegin;
    }

    public void setLimitBegin(int limitBegin) {
        this.limitBegin = limitBegin;
    }

    public int getLimitEnd() {
        return limitEnd;
    }

    public void setLimitEnd(int limitEnd) {
        this.limitEnd = limitEnd;
    }

    public Date getBeginTime() {
        return beginTime;
    }

    public void setBeginTime(Date beginTime) {
        this.beginTime = beginTime;
    }

    public Date getEndTime() {
        return endTime;
    }

    public void setEndTime(Date endTime) {
        this.endTime = endTime;
    }


}
