<template>
    <div class="jifeidanwei-cont">
        <main-layout ref="mainLay" :transfer-customer-height="45">
            <!-- 1.头部 -->
            <div slot="header">
                <!-- 1.第一行 -->
                <mainForm
                    ref="queryForm"
                    :model="queryInfo"
                    :transfer-mobile-collapse="true"
                    label-width="120px"
                >
                    <el-row>
                        <el-col :span="7">
                            <form-input
                                :transfer-form-input="queryInfo"
                                transfer-title="业务编号"
                                transfer-form-prop-key="businessCode"
                                :transfer-disabled="false"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-input
                                :transfer-form-input="queryInfo"
                                transfer-title="表名"
                                transfer-form-prop-key="tableName"
                                :transfer-disabled="false"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-input
                                :transfer-form-input="queryInfo"
                                transfer-title="主键"
                                transfer-form-prop-key="businessId"
                                :transfer-disabled="false"
                            />
                        </el-col>
                    </el-row>
                    <el-row>
                        <el-col :span="7">
                            <form-input
                                :transfer-form-input="queryInfo"
                                transfer-title="内容"
                                transfer-form-prop-key="content"
                                :transfer-disabled="false"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-drop-list
                                :transfer-form-input="queryInfo"
                                transfer-form-prop-key="creator"
                                :transfer-disabled= 'false'
                                transfer-title="员工"
                                :sub-transfer-clean='true'
                                :transfer-fuzzy-dymic-query="{
                                    url: '/SeawinWebappBase/employee/listAllEmployeeName.do',
                                    params: {
                                    },
                                    method: 'post'
                                }"
                                @actionEventManager="getCreator"
                            />
                        </el-col>
                    </el-row>
                </mainForm>
                <!-- <div class = "button-container"> -->
                <!-- </div> -->
            </div>
            <main-button
                slot = 'right'
                style="margin-left: 10px"
                transfer-title="查询"
                @actionEventManager="searchEventManager"
            />
            <main-button
                slot = 'right'
                style="margin-left: 10px"
                transfer-title="重置"
                @actionEventManager="resetEventManager"
            />
            <div slot="body" style="height: 100%">
                <main-table
                  ref="maintable"
                  :transfer-page-control="{
                    show: true,
                    toolbar: false,
                    pageSize: queryInfo.pageSize,
                    pageNumber: queryInfo.pageNumber,
                    totalCount: queryInfo.total
                  }"
                  :transfer-mutable-select-style="true"
                  :transfer-table-header="tableHeaderList"
                  :transfer-table-datas="tableDataSource"
                  :transfer-header-net-load = "0"
                  @actionEventManager="tableActionEventManager"
                />
            </div>
        </main-layout>
    </div>
</template>

<script>

import MainLayout from '@/components/CustomerWidge/MainWidget/MainLayout.vue'
import FormDropList from '@/components/CustomerWidge/FormWidget/formDropList.vue'
import MainTable from '@/components/CustomerWidge/MainWidget/mainTable.vue'
import FormInput from '@/components/CustomerWidge/FormWidget/formInput.vue'
import {getLogList } from '@/api/system.js'
import moment from 'moment'

export default {
    components: {
        MainLayout,
        FormDropList,
        MainTable,
        FormInput
    },

    data() {
        return {
            queryInfo: {
                businessCode:'',// 业务编号
                tableName: '',// 表名
                businessId: '',// 主键
                content: '',// 内容
                creator: '',// 员工
                pageSize: 10,
                pageNumber: 1,
                total: 0
            },
            // 1.列表的头部
            tableHeaderList: [],
            // 2.列表的数据源
            tableDataSource: [],
        };
    },

    methods: {
        //添加头部
        addHeader(){
            var headerItemList = [
                { key: 'businessCode', title: '业务编号', width:200},
                { key: 'creatorName', title: '操作用户', width:100},
                { key: 'typeName', title: '操作类型', width:100},
                { key: 'tableName', title: '表', width:200},
                { key: 'businessId', title: '主键', width:150 },
                { key: 'content', title: '操作内容', width:350 },
                { key: 'createTime', title: '操作时间', width:200 }
            ]
            this.tableHeaderList = headerItemList
        },
        getTableData() {
            const params = {
                start: (this.queryInfo.pageNumber - 1) * this.queryInfo.pageSize,
                length: this.queryInfo.pageSize,
                search: {
                    value: '',
                    regex: false
                },
                keys: JSON.stringify({ ...this.queryInfo })
            }
            getLogList(params)
                .then(res => {
                if (res) {
                    const data = res.aaData || []
                    this.tableDataSource = data.map(item => {
                        console.log("item",item.createTime)
                        item.createTime = item.createTime ? 
                            moment(item.createTime).format('YYYY-MM-DD HH:mm:ss') : item.createTime
                        return item
                    })
                    // this.tableDataSource = data
                    // console.log("item2",item)
                    this.queryInfo.total = res.recordsTotal || 0
                }
                })
                .catch(res => {})
        },
        getCreator(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.creator = item.showValue
            }
            else if (method === 'clear') {
                this.queryInfo.creator = ''
            }
        },  
        // 查询
        searchEventManager() {
            this.queryInfo.pageNumber = 1
            this.getTableData()
        },
        // 重置
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
            }
        },
    },

    created() {
        this.addHeader()
        this.initQueryInfo = JSON.parse(JSON.stringify(this.queryInfo))
    },

    mounted() {
        // Code to run when the component is mounted
    },
};
</script>

<style lang="scss" scoped>
    .jifeidanwei-cont {
      .queryLine {
        width: 100%;
        > form {
          width: 100%;
        }
      }
    }

    .button-container {
      display: flex;
      justify-content: center;
      margin-top: 10px;
    }
</style>