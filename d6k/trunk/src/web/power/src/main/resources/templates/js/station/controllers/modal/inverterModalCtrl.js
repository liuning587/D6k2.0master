/**
 * Created by zyj on 2017/6/14.
 */


DaQOCloud.controller('inverterModalCtrl',['$scope','$http','$rootScope','$filter','stationInfoService',function($scope, $http, $rootScope,$filter,stationInfoService) {


    $scope.loadInverterInfo = function () {

        var stationId = $("#station option:selected").val();
        if (isNaN(stationId)) {
            return;
        }

        $http({
        method: 'GET',
        data:'zyj',
        dataType: 'json',
        url: '/stationInfo/'+ stationId + '/inverter'
        }).success(function (data, status) {

            var tableObj = $('#inverterID');
            tableObj.empty();
            tableObj.append(data.strData);
        }).error(function (data, status) {
            alert("loadInverterInfo error");
        })
    };


    stationInfoService.getStationInfo().then(function (result) {
        $scope.stationInfoList = result;
    });
}]);