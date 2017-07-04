package com.entity.StationInfo;

/**
 * Created by zyj on 2017/6/23.
 */
public class RealtimeCurveInfo {

    public String getStrName() {
        return strName;
    }

    public void setStrName(String strName) {
        this.strName = strName;
    }

    public String getStrTargeName() {
        return strTargeName;
    }

    public void setStrTargeName(String strTargeName) {
        this.strTargeName = strTargeName;
    }

    public Integer getnID() {
        return nID;
    }

    public void setnID(Integer nID) {
        this.nID = nID;
    }

    private Integer nID;
    private String strName;
    private String strTargeName ;
}
