/**
 * Created by zyj on 2017/6/14.
 */


DaQOCloud.controller('reportOfYearModalCtrl',['$scope','$http','$rootScope','$filter','realtimeCurveService',function($scope, $http, $rootScope,$filter,realtimeCurveService) {

    $(function () {
        timeInit();
        showDevice();
        showPoint();
    });

    function timeInit() {
        var myDate = new Date();
        var reportTime = myDate.toLocaleString();
        $('#reportTime').val(reportTime);
    }

    $scope.prevTime = function () {
        var newDate = new Date(Date.parse($('#reportTime').val().replace(/-/g, "/")));
        newDate.setDate(newDate.getDate() - 1);

        $('#reportTime').val(newDate.toLocaleString());
    };

    $scope.currentTime = function () {
        timeInit();
    };

    $scope.nextTime = function () {
        var newDate = new Date(Date.parse($('#reportTime').val().replace(/-/g, "/")));
        var today = new Date();

        if (newDate.toLocaleString() >= today.toLocaleString()) {
            return;
        }

        newDate.setDate(newDate.getDate() + 1);
        $('#reportTime').val(newDate.toLocaleString());
    };

    var g_deviceName = "";

    $scope.reportSearch = function () {
        var tableObj = $('#reportTable');
        tableObj.empty();

        var stationName = $("#station option:selected").text();
        var deviceName = $("#device option:selected").text();
        g_deviceName = deviceName;
        var date = new Date();

        var head = "<div class = 'ReportArea'>";
        head += "<table id='dayDataOfReport' border=\"1\" bordercolor='grey'>";
        head += "<caption style='color: #281ef0'>报表信息： 厂站：【" + stationName + "】 设备：【" + deviceName + "】 查询时间(" + date.toLocaleString() + ")</caption>";
        head += "<thead>";
        head += "<tr style='font-weight : bold' class='table-menu'> <td colspan='17' style='text-align:center;'>"+"日报表【" + stationName + "】【" + deviceName + "】" + "</td></tr>";
        head += "<tr style='font-weight : bold' class=\"table-menu\">";
        head += "<td style='text-align:center;'>测点\\时间</td>";
        head += "<td style='text-align:center;'>6:00</td>";
        head += "<td style='text-align:center;'>7:00</td>";
        head += "<td style='text-align:center;'>8:00</td>";
        head += "<td style='text-align:center;'>9:00</td>";
        head += "<td style='text-align:center;'>10:00</td>";
        head += "<td style='text-align:center;'>11:00</td>";
        head += "<td style='text-align:center;'>12:00</td>";
        head += "<td style='text-align:center;'>13:00</td>";
        head += "<td style='text-align:center;'>14:00</td>";
        head += "<td style='text-align:center;'>15:00</td>";
        head += "<td style='text-align:center;'>16:00</td>";
        head += "<td style='text-align:center;'>17:00</td>";
        head += "<td style='text-align:center;'>18:00</td>";
        head += "<td style='text-align:center;'>19:00</td>";
        head += "<td style='text-align:center;'>20:00</td>";
        head += "<td style='text-align:center;'>单位</td>";
        head += "</tr>";
        head += "</thead>";
        head += "<tbody>";
        head += "</tbody>";
        head += "</table>";
        head += "</div>";

        tableObj.append(head);

        var tableObjDay = $('#dayDataOfReport');
        for (var i = 0; i < 10; i++) {
            var tr = "<tr class=\"table-data\">";
            // 测点的值
            tr += "<td style='text-align:center;'>" + "A相电流" + "</td>";
            tr += "<td style='text-align:center;'>" + 6 + "</td>";
            tr += "<td style='text-align:center;'>" + 7 + "</td>";
            tr += "<td style='text-align:center;'>" + 8 + "</td>";
            tr += "<td style='text-align:center;'>" + 9 + "</td>";
            tr += "<td style='text-align:center;'>" + 10 + "</td>";
            tr += "<td style='text-align:center;'>" + 11 + "</td>";
            tr += "<td style='text-align:center;'>" + 12 + "</td>";
            tr += "<td style='text-align:center;'>" + 13 + "</td>";
            tr += "<td style='text-align:center;'>" + 14 + "</td>";
            tr += "<td style='text-align:center;'>" + 15 + "</td>";
            tr += "<td style='text-align:center;'>" + 16 + "</td>";
            tr += "<td style='text-align:center;'>" + 17 + "</td>";
            tr += "<td style='text-align:center;'>" + 18 + "</td>";
            tr += "<td style='text-align:center;'>" + 19 + "</td>";
            tr += "<td style='text-align:center;'>" + 20 + "</td>";
            tr += "<td style='text-align:center;'>" + "A" + "</td>";

            tr += "</tr>";
            tableObjDay.append(tr);
        }
        // $http({
        //     method: 'POST',
        //     data:'zyj',
        //     url: '/hisDataOfCurve/shortCurve'
        // }).success(function (data, status) {
        //     console.log("succ");
        // }).error(function (data, status) {
        //     alert("error");
        // })
    };

    $scope.exportReport = function () {
        var deviceName = g_deviceName;
        if (deviceName == "") {
            return;
        }
        var date = new Date();
        var fileName = "日报表--"+deviceName+"("+date.toLocaleString()+")";

        $(".ReportArea").table2excel({

            // 导出的Excel文档的名称
            name: "Excel Document Name",

            filename: fileName
        })
    };

    function findParentElement(element) {
        return $(element).parent();
    }

    function findNextElement(element) {
        return $(element).parent().next().next().children();
    }

    function handleDatatypeOption(element, data) {
        element.html("");
        // element.append("<option>请选择</option>");
        for (var i = 0; i< data.length; ++i) {
            element.append("<option value='" + data[i].nID  + "'>" + data[i].strName + "</option>");
        }
    }

    function showDevice() {
        $('select[name=station]').change(function (event) {
            var $select = findParentElement(event.target);
            var $nextElement = findNextElement(event.target);
            var stationId = $select.find("option:selected").get(0).value;

            $http({
                method: 'GET',
                data:'zyj',
                async: false,
                url: '/historyData/station/'+ stationId + '/report'
            }).success(function (data, status) {
                console.log(data.length);

                handleDatatypeOption($nextElement, data);

                pointHandle();

            }).error(function (data, status) {
                alert("error");
            })
        });
    }

    function pointHandle() {

        var deviceName = $("#device").innerHTML;
        var deviceId = $("#device").get(0).value;
        var stationName = $("#station").innerHTML;
        var stationId = $("#station").get(0).value;

        if (stationName === '请选择' || deviceName === '请选择') {
            return;
        }
        $http({
            method: 'GET',
            data:'zyj',
            async: false,
            url: '/historyData/station/'+ stationId + '/' + deviceId + '/report'
        }).success(function (data, status) {
            console.log(data.length);

            handleDatatypeOption($("#point"), data);

        }).error(function (data, status) {
            alert("error");
        })
    }

    function showPoint() {
        $('select[name=device]').change(function (event) {
            var $select = findParentElement(event.target);
            var $nextElement = findNextElement(event.target);
            var deviceName = $select.find("option:selected")[0].innerHTML;
            var deviceId = $select.find("option:selected").get(0).value;
            if (deviceName === '请选择') {
                return;
            }

            var stationId = $("#station").get(0).value;
            $http({
                method: 'GET',
                data:'zyj',
                async: false,
                url: '/historyData/station/'+ stationId + '/' + deviceId + '/report'
            }).success(function (data, status) {
                console.log(data.length);

                handleDatatypeOption($nextElement, data);

            }).error(function (data, status) {
                alert("error");
            })
        });
    }

}]);