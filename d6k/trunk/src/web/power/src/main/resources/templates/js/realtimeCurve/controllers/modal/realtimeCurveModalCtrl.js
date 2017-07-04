/**
 * Created by zyj on 2017/6/14.
 */


DaQOCloud.controller('realtimeCurveModalCtrl',['$scope','$http','$rootScope','$filter','realtimeCurveService',function($scope, $http, $rootScope,$filter,realtimeCurveService) {

    $(function () {
        showDataType();
    });
    // 基于准备好的dom，初始化echarts实例
    var myChart = echarts.init(document.getElementById('realCurve'));

    function loadPieOfVolume(small, big) {

        // 指定图表的配置项和数据
        var option = {
            tooltip : {
                trigger: 'axis'
            },
            legend: {
                data:['实际值','预测值']
            },
            toolbox: {
                show : true,
                feature : {
                    mark : {show: true},
                    dataZoom : {show: true},
                    dataView : {show: true},
                    magicType : {show: true, type: ['line', 'bar', 'stack', 'tiled']},
                    restore : {show: true},
                    saveAsImage : {show: true}
                }
            },
            calculable : true,
            dataZoom : {
                show : true,
                realtime : true,
                start : 20,
                end : 80
            },
            xAxis : [
                {
                    type : 'category',
                    boundaryGap : false,
                    data : function (){
                        var list = [];
                        for (var i = 1; i <= 30; i++) {
                            list.push('2013-03-' + i);
                        }
                        return list;
                    }()
                }
            ],
            yAxis : [
                {
                    type : 'value'
                }
            ],
            series : [
                {
                    name:'实际值',
                    type:'line',
                    smooth:true,  //这句就是让曲线变平滑的
                    data:function (){
                        var list = [];
                        for (var i = 1; i <= 30; i++) {
                            list.push(Math.round(Math.random()* 30));
                        }
                        return list;
                    }()
                },
                {
                    name:'预测值',
                    type:'line',
                    smooth:true,  //这句就是让曲线变平滑的
                    data:function (){
                        var list = [];
                        for (var i = 1; i <= 30; i++) {
                            list.push(Math.round(Math.random()* 10));
                        }
                        return list;
                    }()
                }
            ]
        };



        // 使用刚指定的配置项和数据显示图表。
        myChart.setOption(option);
    }



    $scope.doFilter = function () {
        console.log('loading......');
        $http({
            method: 'POST',
            data:'zyj',
            url: '/hisDataOfCurve/shortCurve'
        }).success(function (data, status) {
            console.log(data.predict_time);
            loadPieOfVolume(100,50);
        }).error(function (data, status) {
            alert("error");
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

    function showDataType() {
        $('select[name=station]').change(function (event) {
            var $select = findParentElement(event.target);
            var $nextElement = findNextElement(event.target);
            var stationName = $select.find("option:selected").val();
            var stationId = $select.find("option:selected").get(0).value;

            $http({
                method: 'GET',
                data:'zyj',
                async: false,
                url: '/realtimeCurve/station/'+ stationId + '/dataType'
            }).success(function (data, status) {
                console.log(data.length);

                handleDatatypeOption($nextElement, data);

            }).error(function (data, status) {
                alert("error");
            })
        });
    }

    //浏览器大小改变时重置大小
    window.onresize = function () {
        myChart.resize();
    };

}]);