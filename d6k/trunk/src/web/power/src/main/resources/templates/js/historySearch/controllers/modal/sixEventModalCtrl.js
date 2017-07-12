/**
 * Created by zyj on 2017/6/14.
 */


DaQOCloud.controller('sixEventModalCtrl',['$scope','$http','$rootScope','$filter','realtimeCurveService',function($scope, $http, $rootScope,$filter,realtimeCurveService) {

    $(function () {
        timeInit();
        pagerSizeChange();
    });

    function timeInit() {
        var newDate = new Date();

        var year = newDate.getFullYear();
        var month = newDate.getMonth() + 1;
        var strDate = newDate.getDate();
        if (month >= 1 && month <= 9) {
            month = "0" + month;
        }
        if (strDate >= 0 && strDate <= 9) {
            strDate = "0" + strDate;
        }
        var endTime = year + "-" + month + "-" + strDate+" "+newDate.getHours()+":"+newDate.getMinutes()+":"+newDate.getSeconds();

        $('#userEventEndTime').val(endTime);

        strDate -= 1;
        var beginTime = year + "-" + month + "-" + strDate+" "+newDate.getHours()+":"+newDate.getMinutes()+":"+newDate.getSeconds();

        $('#userEventBeginTime').val(beginTime);
    }


    $scope.userEventSearch = function () {
        userEventSearch(1, false);
    };

    $scope.exportUserEvent = function () {

        var date = new Date();
        var fileName = "用户事件("+date.toLocaleString()+")";

        $(".ReportArea").table2excel({

            // 导出的Excel文档的名称
            name: "Excel Document Name",

            filename: fileName
        })
    };

    function userEventSearch(pageIndex, change) {
        if (pageIndex <= 0) {
            return;
        }

        var tableObj = $('#userEventTable');
        tableObj.empty();

        var beginTime = $('#userEventBeginTime').val();
        var endTime = $('#userEventEndTime').val();

        beginTime = beginTime.replace(/-/g,"/");
        endTime = endTime.replace(/-/g,"/");

        var date = new Date();

        var head = "<div class = 'Area'>";
        head += "<table id='userEventSearch' border=\"1\" bordercolor='grey'>";
        head += "<thead>";
        head += "<tr style='font-weight : bold' class='table-menu'> <td colspan='5' style='text-align:center;'>用户事件</td></tr>";
        head += "<tr style='font-weight : bold' class=\"table-menu\">";
        head += "<td style='text-align:center;'>ID</td>";
        head += "<td style='text-align:center;'>用户</td>";
        head += "<td style='text-align:center;'>类型</td>";
        head += "<td style='text-align:center;'>时间</td>";
        head += "<td style='text-align:center;'>内容</td>";
        head += "</tr>";
        head += "</thead>";
        head += "<tbody>";
        head += "</tbody>";
        head += "</table>";
        head += "</div>";

        tableObj.append(head);

        var pageSize = $("#pagerSize option:selected").val();

        $http({
            method: 'POST',
            params: {
                beginTime:beginTime,
                endTime:endTime,
                pageSize:pageSize,
                pageIndex:pageIndex
            },
            url: '/historySearch/userEvent'
        }).success(function (data, status) {
            console.log("userEventSearch succ");
            var tableObjDay = $('#userEventSearch');
            for (var i = 0; i < data.listUserEvent.length; i++) {
                var tr = "<tr class=\"table-data\">";
                // 测点的值
                var date = new Date(data.listUserEvent[i].time);

                tr += "<td style='text-align:center;'>" + data.listUserEvent[i].id + "</td>";
                tr += "<td style='text-align:center;'>" + data.listUserEvent[i].username + "</td>";
                tr += "<td style='text-align:center;'>" + data.listUserEvent[i].type + "</td>";
                tr += "<td style='text-align:center;'>" + date.toLocaleString() + "</td>";
                tr += "<td style='text-align:center;'>" + data.listUserEvent[i].content + "</td>";

                tr += "</tr>";
                tableObjDay.append(tr);
            }

            if (change) {
                pagerHandle(data.recordCnt, pageSize, true);
            } else {
                pagerHandle(data.recordCnt, pageSize, false);
            }


        }).error(function (data, status) {
            alert("userEventSearch error");
        })
    }

    function pagerHandle(recordCnt, pageSize, change) {
        if (recordCnt <= 0 || pageSize < 0 || change) {
            return;
        }

        $(".userEventPager").pagination({
            recordCount: recordCnt,       //总记录数
            pageSize: pageSize,           //一页记录数
            onPageChange: function (pageIndex) {
                //document.title = pageIndex;
                userEventSearch(pageIndex, true);
            }
        });
    }

    function pagerSizeChange() {
        $('select[name=pagerSize]').change(function (event) {

            userEventSearch(1, false);
        });
    }

}]);
