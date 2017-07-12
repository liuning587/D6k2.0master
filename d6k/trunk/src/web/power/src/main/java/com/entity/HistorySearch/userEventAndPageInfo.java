package com.entity.HistorySearch;

import com.entity.user_event;

import java.util.Date;
import java.util.List;

/**
 * Created by zyj on 2017/7/6.
 */
public class userEventAndPageInfo {

    public List<user_event> getListUserEvent() {
        return listUserEvent;
    }

    public void setListUserEvent(List<user_event> listUserEvent) {
        this.listUserEvent = listUserEvent;
    }

    public Integer getRecordCnt() {
        return recordCnt;
    }

    public void setRecordCnt(Integer recordCnt) {
        this.recordCnt = recordCnt;
    }

    List<user_event> listUserEvent;

    private Integer recordCnt;
}
