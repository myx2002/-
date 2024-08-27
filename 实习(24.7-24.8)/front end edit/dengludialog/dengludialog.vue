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
                            <form-date-time
                                :transfer-form-input="queryInfo"
                                transfer-form-prop-key="startLoginTime"
                                transfer-title="登录起始时间"
                                transfer-date-type="date"
                                transfer-value-format="yyyy-MM-dd"
                                @actionEventManager="getStartTime"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-date-time
                                :transfer-form-input="queryInfo"
                                transfer-form-prop-key="endLoginTime"
                                transfer-title="登录截止时间"
                                transfer-date-type="date"
                                transfer-value-format="yyyy-MM-dd"
                                @actionEventManager="getEndTime"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-input
                                :transfer-form-input="queryInfo"
                                transfer-title="用户账号"
                                transfer-form-prop-key="authenticationAccount"
                                :transfer-disabled="false"
                            />
                        </el-col>
                    </el-row>
                </mainForm>
                <!-- <div class = "button-container"> --> 
                <!-- </div> -->
            </div>
            <main-button
                slot = "right"
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
import MainTable from '@/components/CustomerWidge/MainWidget/mainTable.vue'
import FormInput from '@/components/CustomerWidge/FormWidget/formInput.vue'
import FormDateTime from '@/components/CustomerWidge/FormWidget/formDateTime.vue'
import {getLoginLogList } from '@/api/system.js'
import moment from 'moment'

export default {
    components: {
        MainLayout,
        FormDateTime,
        MainTable,
        FormInput
    },

    data() {
        return {
            queryInfo: {
                authenticationAccount:'',//用户账号
                startLoginTime:'',//登录起始时间
                endLoginTime:'',//登录截止时间

                pageSize: 20,
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
                { key: 'loginLogId', title: '日志编号', width:100},
                { key: 'username', title: '用户名称', width:100},
                { key: 'authenticationAccount', title: '用户账号', width:100},
                { key: 'loginIp', title: '访问IP', width:130},
                { key: 'type', title: '操作类型', width:120 },
                { key: 'organizationStructrueName', title: '登录公司', width:250 },
                { key: 'content', title: '企业账号', width:120 },
                { key: 'description', title: '登陆人', width:100 },
                { key: 'loginTime', title: '操作时间', width:170 }
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
            getLoginLogList(params)
                .then(res => {
                if (res) {
                    const data = res.aaData || []
                    // this.tableDataSource = data.map(item => {
                    //     console.log("item",item.createTime)
                    //     item.createTime = item.createTime ? 
                    //         moment(item.createTime).format('YYYY-MM-DD HH:mm:ss') : item.createTime
                    //     return item
                    // })
                    this.tableDataSource = data
                    this.queryInfo.total = res.recordsTotal || 0
                }
                })
                .catch(res => {})
        },
        //获取起始时间
        getStartTime(method, item, cname, realitem) {
            console.log(method, item, cname,realitem)
            if (method === 'change') {
                this.queryInfo.startLoginTime = realitem.split(' ')[0]
            }
            else if (method === 'clear') {
                this.queryInfo.startLoginTime = ''
            }
        },  
        //获取截止时间
        getStartTime(method, item, cname, realitem) {
            console.log(method, item, cname,realitem)
            // console.log("item",realitem)
            if (method === 'change') {
                this.queryInfo.endLoginTime = realitem.split(' ')[0]
            }
            else if (method === 'clear') {
                this.queryInfo.endLoginTime = ''
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
        this.getTableData()
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