package com.entity;

import org.springframework.stereotype.Service;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Created by zyj on 2017/6/30.
 */
@Service("inverterEntity")
public class Inverter {

    public String getM_strElementTemplate() {
        return m_strInverterWholeElemTemplate;
    }

    public void setM_strElementTemplate(String m_strElementTemplate) {
        this.m_strInverterWholeElemTemplate = m_strElementTemplate;
    }

    // 逆变器整体模板
    private String m_strInverterWholeElemTemplate;

    // 逆变器量测模板
    private String m_strInverterMeasureElemTemplate;

    // 逆变器遥信模板(状态量)
    private String m_strInverterStatusElemTemplate;

    // 从模板conf中获取元素
    public String getInverterWholeFromTemplateConf() {
        // 文件名先写死，后续改成从数据库中读取
        String strFileName = "inverter-whole.conf";
        if (m_strInverterWholeElemTemplate == null){
            String test = getClass().getClassLoader().getResource("").getPath();
            String strFileNamePath = getClass().getClassLoader().getResource("").getPath() + "conf" + File.separator + strFileName;
            m_strInverterWholeElemTemplate = getConf(strFileNamePath);
        }
        return m_strInverterWholeElemTemplate;
    }

    // 从模板conf中获取元素
    public String getInverterMeasureElemFromTemplateConf() {
        // 文件名先写死，后续改成从数据库中读取
        String strFileName = "inverter-measurement-element.conf";
        if (m_strInverterMeasureElemTemplate == null){
            String test = getClass().getClassLoader().getResource("").getPath();
            String strFileNamePath = getClass().getClassLoader().getResource("").getPath() + "conf" + File.separator + strFileName;
            m_strInverterMeasureElemTemplate = getConf(strFileNamePath);
        }
        return m_strInverterMeasureElemTemplate;
    }

    // 从模板conf中获取元素
    public String getInverterStatusElemFromTemplateConf() {
        // 文件名先写死，后续改成从数据库中读取
        String strFileName = "inverter-status-element.conf";
        if (m_strInverterStatusElemTemplate == null){
            String test = getClass().getClassLoader().getResource("").getPath();
            String strFileNamePath = getClass().getClassLoader().getResource("").getPath() + "conf" + File.separator + strFileName;
            m_strInverterStatusElemTemplate = getConf(strFileNamePath);
        }
        return m_strInverterStatusElemTemplate;
    }

    // 创建逆变器整体元素
    public String createInverterWholeElem() {
        // 获取逆变器整体模板
        String strInverterWholeElement = getInverterWholeFromTemplateConf();

        String strMeasureElem = "";
        strMeasureElem += createInverterMeasureElem("当日发电量", "mean-green-bg");
        strMeasureElem += createInverterMeasureElem("月发电量", "mean-red-bg");
        strMeasureElem += createInverterMeasureElem("总发电量", "mean-orange-bg");

        String strStatusElem = createInverterStatusElem();

        String strInverterTile = "场站x-设备x-1#逆变器";
        if (strInverterWholeElement.contains("INVERTER_TITLE")) {
            strInverterWholeElement = strInverterWholeElement.replace("INVERTER_TITLE", strInverterTile);
        }

        if (strInverterWholeElement.contains("INVERTER_MEASUREMENT_ELEMENT")) {
            strInverterWholeElement = strInverterWholeElement.replace("INVERTER_MEASUREMENT_ELEMENT", strMeasureElem);
        }

        if (strInverterWholeElement.contains("INVERTER_STATUS_ELEMENT")) {
            strInverterWholeElement = strInverterWholeElement.replace("INVERTER_STATUS_ELEMENT", strStatusElem);
        }

        return strInverterWholeElement;
    }

    // 创建逆变器量测元素
    private String createInverterMeasureElem(String strDataName, String strExtra) {
        // 获取逆变器整体模板
        String strInverterMeasureTemplate = getInverterMeasureElemFromTemplateConf();

        // todo:后续改成从数据库中读取信息,先写死

        // mean-green-bg
        if (strInverterMeasureTemplate.contains("EXTRA_STRING_1")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("EXTRA_STRING_1", strExtra);
        }

        //String strDataName = "日发电量";
        if (strInverterMeasureTemplate.contains("DATA_NAME")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("DATA_NAME", strDataName == null ? "" : strDataName);
        }

        if (strInverterMeasureTemplate.contains("DATA_TEMPLATE_INDEX")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("DATA_TEMPLATE_INDEX", "1");
        }

        if (strInverterMeasureTemplate.contains("STATION_ID")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("STATION_ID", "1");
        }

        if (strInverterMeasureTemplate.contains("DEVICE_ID")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("DEVICE_ID", "1");
        }

        if (strInverterMeasureTemplate.contains("DATA_TARGETNAME")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("DATA_TARGETNAME", "1");
        }

        if (strInverterMeasureTemplate.contains("DATA_MEANS_TYPE")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("DATA_MEANS_TYPE", "1");
        }

        if (strInverterMeasureTemplate.contains("STRING_UNIT")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("STRING_UNIT", "kWh");
        }

        //String strDesc = "日发电量";
        if (strInverterMeasureTemplate.contains("STRING_NAME")) {
            strInverterMeasureTemplate = strInverterMeasureTemplate.replace("STRING_NAME", strDataName);
        }

        return strInverterMeasureTemplate;
    }

    // 创建逆变器遥信元素
    private String createInverterStatusElem() {
        // 获取逆变器整体模板
        String strInverterStatusTemplate = getInverterStatusElemFromTemplateConf();

        // todo:后续改成从数据库中读取信息,先写死

        String strDataName = "日发电量";
        if (strInverterStatusTemplate.contains("DATA_NAME")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("DATA_NAME", strDataName == null ? "" : strDataName);
        }

        if (strInverterStatusTemplate.contains("DATA_TEMPLATE_INDEX")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("DATA_TEMPLATE_INDEX", "1");
        }

        if (strInverterStatusTemplate.contains("STATION_ID")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("STATION_ID", "1");
        }

        if (strInverterStatusTemplate.contains("DEVICE_ID")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("DEVICE_ID", "1");
        }

        if (strInverterStatusTemplate.contains("DATA_TARGETNAME")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("DATA_TARGETNAME", "1");
        }

        if (strInverterStatusTemplate.contains("DATA_MEANS_TYPE")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("DATA_MEANS_TYPE", "1");
        }

        String strDesc = "逆变器状态";
        if (strInverterStatusTemplate.contains("STRING_NAME")) {
            strInverterStatusTemplate = strInverterStatusTemplate.replace("STRING_NAME", strDesc);
        }

        return strInverterStatusTemplate;
    }

    // 读文件
    private static String getConf(String fileName) {
        File file = new File(fileName);
        Long fileLength = file.length();
        byte[] fileContent = new byte[fileLength.intValue()];
        try {
            FileInputStream in = new FileInputStream(file);
            in.read(fileContent);
            in.close();

            return new String(fileContent);

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return "";
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }
}
