<template>
    <div class="jifeidanwei-cont">
        <main-layout ref="mainLay" :transfer-customer-height="45">
            <!-- 1.头部 -->
            <div slot="header">
                <mainForm
                    ref="queryForm"
                    :model="queryInfo"
                    :transfer-mobile-collapse="true"
                    label-width="120px"
                >
                    <el-row>
                        <!-- prop-key对映 -->
                        <el-col :span="7">
                            <form-input
                                :transfer-form-input="queryInfo"
                                transfer-title="备注"
                                transfer-form-prop-key="remark"
                                :transfer-disabled="false"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-date-time
                                :transfer-form-input="queryInfo"
                                transfer-form-prop-key="pasiCreatTime"
                                transfer-title="创建时间"
                                transfer-date-type="daterange"
                                transfer-value-format="yyyy-MM-dd"
                                transfer-input-width = 250
                                @actionEventManager="setBuildStart"
                            />
                        </el-col>
                        <el-col :span="7">
                            <form-date-time
                                :transfer-form-input="queryInfo"
                                transfer-form-prop-key="pasiAmendTime"
                                transfer-title="编辑时间"
                                transfer-date-type="daterange"
                                transfer-value-format="yyyy-MM-dd"
                                transfer-input-width = 250
                                @actionEventManager="setFixStart"
                            />
                        </el-col>
                    </el-row>

                </mainForm>
            </div>
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
            <main-button
                slot="left"
                transfer-title="新增"
                style="margin-left: 10px"
                transfer-btn-id="BTN_27140100"
                @actionEventManager="addEventManager"
            />
            <main-button
                slot="left"
                transfer-title="删除"
                style="margin-left: 10px"
                transfer-btn-id="BTN_27140200"
                @actionEventManager="deleteEventManager"
            />
            <main-button
                slot="left"
                transfer-title="修改"
                style="margin-left: 10px"
                transfer-btn-id="BTN_27140300"
                @actionEventManager="editEventManager"
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
                <add-dialog 
                    ref="addDialogRef" 
                    @actionEnd = "getDialogResult"
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
import { getPostList,deletePost } from '@/api/smallProject.js'
import AddDialog from './add.vue'


export default {

    components: {
      MainLayout,
      MainTable,
      FormDateTime,
      FormInput,
      AddDialog
    },

    data() {
        return {
            queryInfo: {
                remark: '', // 备注
                startDate: '', // 创建时间
                endDate: '', // 截止时间
                amendStartDate:'', // 修改起始时间
                amendEndDate:'', // 修改截止时间

                pasiCreatTime: '', // 创建时间
                pasiAmendTime: '', // 修改时间
                pageSize: 20, // 当前页行数量
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
        //添加头部
        addHeader(){
        var headerItemList = [
                { key: 'pasiPosters', title: '海报', width:100, highlight: true },
                { key: 'pasiShareWidth', title: '二维码宽度', width:100},
                { key: 'pasiShareHeight', title: '二维码高度', width:100},
                { key: 'pasiShareX', title: '二维码X', width:100},
                { key: 'pasiShareY', title: '二维码Y', width:100 },
                { key: 'pasiStandbyOne', title: '备注', width:100 },
                { key: 'pasiCreatTime', title: '创建时间', width:200 },
                { key: 'pasiAmendTime', title: '修改时间', width:200 }
            ]
            this.tableHeaderList = headerItemList
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
                    Object.entries(this.queryInfo).filter(([key, value]) => value !== null && value !== undefined && value !== '')
                    )
                )
            }
            getPostList(params)
                .then(res => {
                if (res) {
                    const data = res.aaData || []
                    this.tableDataSource = data
                    this.queryInfo.total = res.recordsTotal || 0
                }
                })
                .catch(res => {})
        },
        //查询
        searchEventManager() {
            this.queryInfo.pageNumber = 1
            this.getTableData()
        },
        //重置
        resetEventManager() {
            this.queryInfo = JSON.parse(JSON.stringify(this.initQueryInfo))
            this.getTableData()
        },
        //获取建立起止时间
        setBuildStart(method, item, cname, realitem) {
            console.log(method, item, cname,realitem)
            if (method === 'change') {
                this.queryInfo.startDate = item[0].split(' ')[0]
                this.queryInfo.endDate = item[1].split(' ')[0]
            }
            else if (method === 'clear') {
                this.queryInfo.startDate = ''
                this.queryInfo.endDate = ''
            }
        }, 
        //获取修改起止时间
        setFixStart(method, item, cname, realitem) {
            console.log(method, item, cname,realitem)
            if (method === 'change') {
                this.queryInfo.amendStartDate = item[0].split(' ')[0]
                this.queryInfo.amendEndDate = item[1].split(' ')[0]
            }
            else if (method === 'clear') {
                this.queryInfo.amendStartDate = ''
                this.queryInfo.amendEndDate = ''
            }
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
            else if(method === 'highlightClick')
            {
                // console.log('点击了高亮')
                let imageUrl = item
                const newWindow = window.open()
                newWindow.document.write(`
                    <div style="display: flex; justify-content: center;">
                        <img src="${imageUrl}" alt="图像显示失败" style="width: 80%;">
                    </div>`);
                newWindow.document.title = "图片显示";
            }
        },

        //删除操作
        deleteEventManager() {
            const check_choose = this.$refs.maintable.actionGetSelectedRow()
            if (!check_choose.length) {
                this.$message.info('请勾选需要删除的数据')
                return
            }
            this.$alertConfirmManager('提示', `确定要删除${check_choose.length}条数据吗?`, '确定', '取消').then(res1 => {
                if (res1 === 'confirm') {
                    console.log(check_choose)
                    const params = {
                        id: check_choose.map(item => item.pasiId).join(',')
                    }
                    // console.log(params)
                    deletePost(params)
                    .then(res => {
                        if (res && res.code === 0) {
                            this.$message.success('删除成功')
                            this.getTableData()
                        }
                    })
                    .catch(res => {})
                } else {
                    this.$message.info('取消操作')
                }
            })
        },
        //新增操作
        addEventManager() {
            this.$refs.addDialogRef.open('add')
        },
        //修改操作
        editEventManager() {
            const check_choose = this.$refs.maintable.actionGetSelectedRow()
            if (!check_choose.length) {
                this.$message.info('请勾选需要修改的数据')
                return
            }
            else if(check_choose.length > 1){
                this.$message.info('只能选择一条数据进行修改')
                return
            }
            else
            {
                this.$refs.addDialogRef.open('edit',check_choose[0])
            }
        },
        //获取弹窗的结果
        getDialogResult(method) {
            if (method === 'ScucessAdd') {
                this.getTableData()
            }
        }

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
</style>