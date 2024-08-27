<template>
    <main-dialog
        ref="templateDialog"
        :transfer-title="editStatus"
        :transfer-width="1100"
        :transfer-btn-list="[]"
        @actionEventManager="mainDialogActionEventManager"
    >
        <el-card class="box-card">
            <div style="width: 100%">
                <mainForm ref="queryForm" :model="queryInfo" label-width="120px">
                    <div class="form-cont" v-if="this.paytype === 'sea' ">
                        <el-row>
                            <el-col :span="8">
                              <form-drop-list
                                  :transfer-form-input="queryInfo"
                                  transfer-form-prop-key="start_port"
                                  :transfer-disabled= 'false'
                                  transfer-title="启运港"
                                  transfer-fuzzy-key="portNameCn"
                                  transfer-sub-title-key="port3code"
                                  :transfer-fuzzy-dymic-query="{
                                    url: 'hm/basedata/basedata/fuzzyQueryPort',
                                    params: { text: '??', type: 'OP | 海港', name: '海运出口海港起运港', length: 20 }
                                  }"
                                  :sub-transfer-clean='true'
                                  transfer-remote-type="net"
                                  @actionEventManager="getStartPort"
                              />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="trans_port"
                                    :transfer-disabled= 'false'
                                    transfer-title="中转港"
                                    :sub-transfer-clean='true'
                                    transfer-remote-type="net"
                                    :transfer-fuzzy-dymic-query="{
                                        url: 'hm/basedata/basedata/fuzzyQueryPort',
                                        params: {
                                            type: 'OP | 海港',
                                            text: '??',
                                            name: '海运出口海港中转港',
                                            length: 20
                                            }
                                    }"
                                    transfer-fuzzy-key="portNameCn"
                                    transfer-sub-title-key="port3code"
                                    @actionEventManager="getTransPort"
                                />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="target_port"
                                    :transfer-disabled= 'false'
                                    transfer-title="目的港"
                                    :sub-transfer-clean='true'
                                    transfer-remote-type="net"
                                    transfer-fuzzy-key="portNameCn"
                                    transfer-sub-title-key="port3code"
                                    :transfer-fuzzy-dymic-query="{
                                        url: 'hm/basedata/basedata/fuzzyQueryPort',
                                        params: { text: '??', type: 'OP | 海港', name: '海运出口海港目的港', length: 20 }
                                    }"
                                    @actionEventManager="getEndPort"
                                />
                            </el-col>
                        </el-row> 
                        <el-row>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="ship_company"
                                    :transfer-disabled= 'false'
                                    transfer-title="船公司"
                                    :sub-transfer-clean='true'
                                    transfer-remote-type="net"
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do',
                                        params: { name: 'basedataVesselParameter', keys: '{}' },
                                        method: 'post'
                                    }"
                                    @actionEventManager="getShipCompany"
                                />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="user_level"
                                    :transfer-disabled= 'false'
                                    transfer-title="客户等级"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do',
                                        params: { name: 'basedataCommonSet_24' }
                                    }"
                                    @actionEventManager="getCusLevel"
                                />
                            </el-col>
                            <div class = "button-container">
                                <main-button
                                  slot="right"
                                  style="margin-left: 10px"
                                  transfer-title="查询"
                                  @actionEventManager="searchEventManager"
                                />
                                <main-button
                                  slot="right"
                                  style="margin-left: 10px"
                                  transfer-title="重置"
                                  @actionEventManager="resetEventManager"
                                />
                            </div>
                        </el-row> 
                    </div>
                    <div class="form-cont" v-if="this.paytype === 'fly' ">
                        <el-row>
                            <el-col :span="8">
                              <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="start_port_fly"
                                    :transfer-disabled= 'false'
                                    transfer-title="起运港"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do?name=basedataPortAP',
                                        params: {
                                        },
                                        method: 'post'
                                    }"
                                    @actionEventManager="getStartPort"
                                />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="target_port_fly"
                                    :transfer-disabled= 'false'
                                    transfer-title="目的港"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do?name=basedataPortAP',
                                        params: {
                                        },
                                        method: 'post'
                                    }"
                                    @actionEventManager="getEndPort"
                                />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="trans_price_level"
                                    :transfer-disabled= 'false'
                                    transfer-title="运价等级"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do?name=freightAirRank',
                                        params: {
                                        },
                                        method: 'post'
                                    }"
                                    @actionEventManager="getPriceLevel"
                                />
                            </el-col>
                        </el-row> 
                        <el-row>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="fly_line"
                                    :transfer-disabled= 'false'
                                    transfer-title="航线"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/basedataServiceLine/getAllLine.do',
                                        params: { draw: 1, start: 0, length: 1000 }
                                    }"
                                    transfer-fuzzy-key="serviceLineNameCn"
                                    @actionEventManager="getAirLine"
                                />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="fly_line_company"
                                    :transfer-disabled= 'false'
                                    transfer-title="航空公司"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do?name=basedataAirline',
                                        params: {
                                        },
                                        method: 'post'
                                    }"
                                    @actionEventManager="getAirLineCompany"
                                />
                            </el-col>
                            <el-col :span="8">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-form-prop-key="user_level"
                                    :transfer-disabled= 'false'
                                    transfer-title="客户等级"
                                    :sub-transfer-clean='true'
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/redisController/listIdNameByName.do',
                                        params: { name: 'basedataCommonSet_24' }
                                    }"
                                    @actionEventManager="getCusLevel"
                                />
                            </el-col>
                        </el-row> 
                        <el-row>
                            <div class = "button-container">
                                <main-button
                                  slot="right"
                                  style="margin-left: 10px"
                                  transfer-title="查询"
                                  @actionEventManager="searchEventManager"
                                />
                                <main-button
                                  slot="right"
                                  style="margin-left: 10px"
                                  transfer-title="重置"
                                  @actionEventManager="resetEventManager"
                                />
                            </div>
                        </el-row>
                    </div>
                </mainForm>
            </div>
            <div style="height: 300px" v-if="this.paytype === 'sea' ">
                <main-table
                    ref="maintable"
                    :transfer-page-control="{
                        show: true,
                        pageSize: queryInfo.pageSize,
                        pageNumber: queryInfo.pageNumber,
                        totalCount: queryInfo.total
                    }"
                    :transfer-table-header="tableHeaderList"
                    :transfer-table-datas="tableDataSource"
                    :transfer-header-net-load = "0"
                    :transfer-no-check="true"
                    @actionEventManager="tableActionEventManager"
                />
            </div>
            <div style="height: 300px" v-if="this.paytype === 'fly' ">
                <main-table
                    ref="maintable_fly"
                    :transfer-page-control="{
                        show: true,
                        pageSize: queryInfo.pageSize,
                        pageNumber: queryInfo.pageNumber,
                        totalCount: queryInfo.total
                    }"
                    :transfer-table-header="tableHeaderList"
                    :transfer-table-datas="tableDataSource"
                    :transfer-header-net-load = "0"
                    :transfer-no-check="true"
                    @actionEventManager="tableActionEventManager"
                />
            </div>
        </el-card>            
    </main-dialog>
</template>

<script>
import MainDialog from '@/components/CustomerWidge/MainWidget/mainDialog.vue'
import FormDropList from '@/components/CustomerWidge/FormWidget/formDropList.vue'
import MainTable from '@/components/CustomerWidge/MainWidget/mainTable.vue'
import { getTransportList, getAirTransportList } from '@/api/smallProject.js'

export default {
    components: {
        MainDialog,
        FormDropList,
        MainTable
    },

    data() {
        return {
            editStatus:"运价选择",
            paytype: '',
            queryInfo: {
                //1.0查询需要-空运
                freightAirRankId:'',
                serviceLineId:'',
                airlineId:'',
                //空运航线
                start_port_fly: '',//起运港
                target_port_fly: '',//目的港
                trans_prince_level: '',//运价等级
                fly_line: '',//航线
                fly_line_company: '',//航空公司
                //1.0查询需要-海运
                loadingPortId:"",
                transshipmentPortId:"",
                dischargingPortId:"",
                vesselParameterId:"",
                customerRankId:"",
                releaseStatus:"已发布",
                //---查询end---
                //海运航线
                start_port: '',//启运港
                trans_port: '',//中转港
                target_port: '',//目的港
                ship_company: '',//船公司
                user_level: '',//客户等级
                pageSize: 10, // 当前页行数量
                pageNumber: 1, // 当前第几页
                total: 0 // 总行数
            },
            // 1.列表的头部
            tableHeaderList: [],
            // 2.列表的数据源
            tableDataSource: [],
        };
    },

    methods: {
        //open函数
        open(type) {
            this.paytype = type
            this.$refs.templateDialog.open()
            this.addHeader()
        },

        addHeader(){
            var headerItemList = [
                { key: 'rowindex', title: '序号', width:50},
                { key: 'loadingPortNameCn', title: '启运港/港区', width:100},
                { key: 'dischargingPortNameCn', title: '目的港/港区', width:100},
                { key: 'releaseStatus', title: '状态', width:100},
                { key: 'freightShippingRankName', title: '运价等级', width:100},
                { key: 'transshipmentPortName', title: '中转港', width:100},
                { key: 'vesselParameterName', title: '船公司', width:100},
                { key: 'serviceLineCode', title: '航线代码'},
                { key: 'sailingDate', title: '船期', width:100},
                { key: 'voyage', title: '船程', width:100},
                { key: 'price20', title: '20GP', width:100},
                { key: 'price40', title: '40GP', width:100},
                { key: 'price40h', title: '40HQ', width:100},
                { key: 'releaseTime', title: '发布时间'},
                { key: 'description', title: '备注'},
                { key: 'baseModel.amenderName', title: '操作人'}
            ];
            var headerItemList_fly = [
                { key: 'airQuotationRankCode', title: '等级价格编号', width:150},
                { key: 'airQuotationCode', title: '基础运价编号', width:150},
                { key: 'releaseStatus', title: '状态', width:70},
                { key: 'freightAirRankName', title: '运价等级', width:100},
                { key: 'customerRankName', title: '客户等级', width:100},
                { key: 'loadingPortName', title: '起运港', width:100},
                { key: 'domesticTransshipmentPortName', title: '国内中转港', width:100},
                { key: 'internationalTransshipmentPortName', title: '国际中转港',width:100},
                { key: 'dischargingPortName', title: '目的港', width:100},
                { key: 'countryName', title: '国家', width:100},
                { key: 'airlineName', title: '航空公司', width:200},
                { key: 'flyingDate', title: '航期', width:200},
                { key: 'serviceLineName', title: '航线', width:70},
                { key: 'flight', title: '航程', width:70},
                { key: 'priceM', title: 'M', width:70},
                { key: 'priceN', title: 'N', width:70},
                { key: 'price45', title: '45+', width:70},
                { key: 'price100', title: '100+', width:70},
                { key: 'price300', title: '300+', width:70},
                { key: 'price500', title: '500+', width:70},
                { key: 'price1000', title: '1000+', width:70},
                { key: 'proportion', title: '比重', width:70},
                { key: 'beginEnableTime', title: '开始', width:150},
                { key: 'endEnableTime', title: '结束', width:70},
                { key: 'customerServiceName', title: '客服', width:100},
                { key: 'releaseTime', title: '发布时间', width:150},
                { key: 'baseModel.amenderName', title: '操作人', width:100},
                { key: 'amendTime', title: '修改日期', width:150},
                { key: 'externalRemark', title: '备注', width:70}
            ];
            if(this.paytype === 'sea'){
                this.tableHeaderList = headerItemList
            }
            else if(this.paytype === 'fly'){
                this.tableHeaderList = headerItemList_fly
            }
        },

        //获取数据
        getTableData() {
            const params = {
                start: (this.queryInfo.pageNumber - 1) * this.queryInfo.pageSize,
                length: this.queryInfo.pageSize,
                search: {
                    value: '',
                    regex: false
                },
                //清除空元素干扰
                keys: JSON.stringify(
                Object.fromEntries(
                    Object.entries(this.queryInfo).filter(([key, value]) => value !== null && 
                                                            value !== undefined && value !== '')
                    )
                )
            }
            if(this.paytype === 'sea'){
                getTransportList(params)
                    .then(res => {
                        if (res) {
                            const data = res.aaData || []
                            this.tableDataSource = data
                            this.queryInfo.total = res.recordsTotal || 0
                        }
                    })
                    //处理失败情况
                    .catch(res => {})
            }
            else if(this.paytype === 'fly'){
                getAirTransportList(params)
                    .then(res => {
                        if (res) {
                            const data = res.aaData || []
                            this.tableDataSource = data
                            this.queryInfo.total = res.recordsTotal || 0
                        }
                    })
                    //处理失败情况
                    .catch(res => {})
            }
            
        },

        //main-dialog操作
        mainDialogActionEventManager(method, item, cname, realitem) {
            console.log(method, item, cname, realitem)
            if (method === 'calheight') {
                this.getTableData()
            }
        },

        //起始港
        getStartPort(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                if(this.paytype === 'sea'){
                    this.queryInfo.start_port = item.showValue + '|' + item.showSubValue
                    this.queryInfo.loadingPortId = item.item.portId
                }
                else if(this.paytype === 'fly'){
                    this.queryInfo.start_port_fly = item.showValue
                    this.queryInfo.loadingPortId = item.showKey
                }
            }
            else if (method === 'clear') {
                this.queryInfo.start_port = ''
                this.queryInfo.loadingPortId = ''
                this.queryInfo.start_port_fly = ''
            }
        },
        //中转港
        getTransPort(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.trans_port = item.showValue + '|' + item.showSubValue
                this.queryInfo.transshipmentPortId = item.item.portId
            }
            else if (method === 'clear') {
                this.queryInfo.trans_port = ''
                this.queryInfo.transshipmentPortId = ''
            }
        },
        //目的港
        getEndPort(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                if(this.paytype === 'fly'){
                    this.queryInfo.target_port_fly = item.showValue
                    this.queryInfo.dischargingPortId = item.showKey
                }
                else if(this.paytype === 'sea'){
                    this.queryInfo.target_port = item.showValue + '|' + item.showSubValue
                    this.queryInfo.dischargingPortId = item.item.portId
                }
            }
            else if (method === 'clear') {
                this.queryInfo.target_port = ''
                this.queryInfo.dischargingPortId = ''
                this.queryInfo.target_port_fly = ''
            }
        },
        //船公司
        getShipCompany(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.ship_company = item.showValue
                this.queryInfo.vesselParameterId = item.showKey
            }
            else if (method === 'clear') {
                this.queryInfo.ship_company = ''
                this.queryInfo.vesselParameterId = ''
            }
        },
        //客户等级
        getCusLevel(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.user_level = item.showValue
                this.queryInfo.customerRankId = item.showKey
            }
            else if (method === 'clear') {
                this.queryInfo.user_level = ''
                this.queryInfo.customerRankId = ''
            }
        },
        //运价等级
        getPriceLevel(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.trans_prince_level = item.showValue
                this.queryInfo.freightAirRankId = item.showKey
            }
            else if (method === 'clear') {
                this.queryInfo.trans_prince_level = ''
                this.queryInfo.freightAirRankId = ''
            }
        },
        //航线
        getAirLine(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.fly_line = item.showValue
                this.queryInfo.serviceLineId = item.item.serviceLineId
            }
            else if (method === 'clear') {
                this.queryInfo.fly_line = ''
                this.queryInfo.serviceLineId = ''
            }
        },
        //航空公司
        getAirLineCompany(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.fly_line_company = item.showValue
                this.queryInfo.airlineId = item.showKey
            }
            else if (method === 'clear') {
                this.queryInfo.fly_line_company = ''
                this.queryInfo.airlineId = ''
            }
        },

        // 查询
        searchEventManager() {
            this.queryInfo.pageNumber = 1
            this.getTableData()
        },
        //重置
        resetEventManager() {
            this.queryInfo = JSON.parse(JSON.stringify(this.initQueryInfo))
            this.getTableData()
        },

        //table操作
        tableActionEventManager(method, item, cname, realitem) {
            console.log(method, item, cname, realitem)
            if (method === 'changePage') {
                // 修改页码(item表示页码)
                this.queryInfo.pageNumber = item
                this.getTableData()
            } else if (method === 'changeSize') {
                // 修改一页多少条数据
                this.queryInfo.pageNumber = 1
                this.queryInfo.pageSize = item
                this.getTableData()
            } else if (method === 'doubleClicked'){
                // console.log(realitem.shippingQuotationCode)
                if(this.paytype === 'sea'){
                   this.$emit('actionChoose','chooseTransPayin', realitem.shippingQuotationCode, realitem.shippingQuotationId,'1') 
                }
                else if(this.paytype === 'fly'){
                    this.$emit('actionChoose','chooseTransPayin', realitem.airQuotationCode, realitem.airQuotationId,'2') 
                    console.log("抛出成功",realitem.airQuotationCode," ",realitem.airQuotationId)
                }
                this.$refs.templateDialog.closeDelegate()
            }
        },
    },

    created() {
        this.initQueryInfo = JSON.parse(JSON.stringify(this.queryInfo))
    },

    mounted() {

    },
};
</script>

<style lang="scss" scoped>
    .button-container {
      display: flex;
      justify-content: flex-end;
      margin-top: 10px;
    }
</style>