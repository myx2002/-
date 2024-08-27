<!-- 修改还没写好 -->
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
            <!-- *****编写表单信息开始****** -->
            <el-row>
              <el-col :span="7">
                <form-drop-list
                    :transfer-form-input="queryInfo"
                    transfer-form-prop-key="couPon"
                    :transfer-disabled= 'false'
                    transfer-title="优惠券"
                    :sub-transfer-clean='true'
                    :transfer-fuzzy-dymic-query="{
                      url: '/SeawinWebappBase/tCouponBanner/getCouponList.do',
                      params: {
                      },
                      method: 'post'
                    }"
                    transfer-fuzzy-key = "name"
                    @actionEventManager="getcouPonType"
                />
              </el-col>
              <el-col :span="7">
                <form-drop-list
                  :transfer-form-input="queryInfo"
                  transfer-form-prop-key="state"
                  :transfer-disabled= 'false'
                  transfer-title="状态"
                  :transfer-fuzzy-fixed="['已发布', '待发布']"
                  :sub-transfer-clean='true'
                  @actionEventManager="getcouPonState"
                />
              </el-col>
              <el-col :span="7">
                <form-drop-list
                  :transfer-form-input="queryInfo"
                  transfer-form-prop-key="activityType"
                  :transfer-disabled= 'false'
                  transfer-title="活动类型"
                  :transfer-fuzzy-fixed="['小程序', '门户','小程序特价','小程序直播']"
                  :sub-transfer-clean='true'
                  @actionEventManager="getcouPonActivityType"
                />
              </el-col>
            </el-row>
            <!-- *****编写表单信息结束****** -->
          </mainForm>
          <!-- <div class = "button-container"> -->
          <!-- </div> -->
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
          transfer-btn-id="BTN_27110100"
          @actionEventManager="addEventManager"
        />
        <main-button
          slot="left"
          transfer-title="删除"
          transfer-btn-id="BTN_27110200"
          @actionEventManager="delEventManager"
        />
        <main-button
          slot="left"
          transfer-title="修改"
          transfer-btn-id="BTN_27110300"
          @actionEventManager="editEventManager"
        />
        <main-button 
          slot="left" 
          transfer-title="发布"
          transfer-btn-id="BTN_27110400"
          @actionEventManager="outEventManager"
        />
        <main-button 
          slot="left" 
          transfer-title="撤回" 
          transfer-btn-id="BTN_27110500"
          @actionEventManager="backEventManager"
        />
        <main-button 
          slot="left" 
          transfer-title="置顶" 
          transfer-btn-id="BTN_27110600"
          @actionEventManager="fixtopEventManager"
        />
        <main-button 
          slot="left" 
          transfer-title="取消置顶" 
          transfer-btn-id="BTN_27110700"
          @actionEventManager="flextopEventManager"
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
          <!-- 新增弹窗 -->
          <add-dialog 
            ref="addDialogRef" 
            @actionEnd="actionEnd_out"
          />
        </div>
      </main-layout>
    </div>
  </template>

<script>
import MainLayout from '@/components/CustomerWidge/MainWidget/MainLayout.vue'
import FormDropList from '@/components/CustomerWidge/FormWidget/formDropList.vue'
import MainTable from '@/components/CustomerWidge/MainWidget/mainTable.vue'
import { getCouponList, deleteCoupon, outCoupon, fixtopCoupon } from '@/api/smallProject.js'
import AddDialog from './add.vue'


export default {
  components: {
      MainLayout,
      FormDropList,
      MainTable,
      AddDialog
  },


  data() {
      return {
      queryInfo: {
          state: '',//状态选择
          bannerStatus:'',//状态选择id（1.0传输需要）
          couPon: '',//优惠券选择
          couponId:'',//优惠券id(1.0传输需要)
          activityType: '',//活动类型选择
          bannerBackupOne:'',//类型搜索（1.0传输）
          pageSize: 20, // 当前页行数量
          pageNumber: 1, // 当前第几页
          total: 0 // 总行数
      },
      // 1.列表的头部
      tableHeaderList: [],
      // 2.列表的数据源
      tableDataSource: [],
      }
  },

  created(){
    this.addHeader()
    //保存queryinfo初始状态
    this.initQueryInfo = JSON.parse(JSON.stringify(this.queryInfo))
  },
  mounted() {
    this.getTableData()
  },

  methods:{
    //add成功刷新数据
    actionEnd_out(method){
      if(method === 'ScucessAdd'){
        this.getTableData()
      }
    },
    //添加头部
    addHeader(){
      var headerItemList = [
        { key: 'coupon_name', title: '优惠券名称', width:300},
        { key: 'banner_img', title: 'banner', highlight: true},
        { key: 'banner_number', title: '剩余券数'},
        { key: 'banner_backup_three', title: '跳转路径', width:350},
        { key: 'banner_status', title: '状态', width:100 },
        { key: 'banner_top', title: '是否置顶', width:100 },
        { key: 'banner_backup_one', title: '活动类型', width:100 }
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
      getCouponList(params)
        .then(res => {
          if (res) {
            const data = res.aaData || []
            this.tableDataSource = data.map(item => {
              item.banner_status = item.banner_status === 1 ? '已发布' : '待发布'
              item.banner_top = item.banner_top === 1 ? '是' : '否'
              //排除奇怪为1跳转路径的出现
              item.banner_backup_three = 
                (item.banner_backup_three !== '1' && item.banner_backup_three !== null) ? item.banner_backup_three : ''
              return item
            })
            this.queryInfo.total = res.recordsTotal || 0
          }
        })
        .catch(res => {})
    },

    //优惠券
    getcouPonType(method, item, cname, realitem) {
      console.log(method, item)
      if (method === 'select') {
        this.queryInfo.couponId = item.item.code
        // console.log(this.queryInfo.couponId)
      }
      else if (method === 'clear') {
        this.queryInfo.couponId = ''
      }
    },

    //优惠券状态
    getcouPonState(method, item, cname, realitem) {
      console.log(method, item)
      if (method === 'select') {
        this.queryInfo.bannerStatus = item.showValue === "已发布" ? 1 : 0
        // console.log(this.queryInfo.stateId)
      }
      else if (method === 'clear') {
        this.queryInfo.bannerStatus = ''
      }
    },

    //活动类型
    getcouPonActivityType(method, item, cname, realitem) {
      console.log(method, item)
      if (method === 'select') {
        this.queryInfo.bannerBackupOne = item.showValue
        // console.log(item)
      }
      else if (method === 'clear') {
        this.queryInfo.bannerBackupOne = ''
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
      }else if(method === 'highlightClick'){
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

    // 新增(按钮默认点击事件)
    addEventManager() {
      this.$refs.addDialogRef.open('')
    },
    // 撤回（发布）
    backEventManager() {
      const check_choose = this.$refs.maintable.actionGetSelectedRow()
      if (!check_choose.length) {
          this.$message.info('请勾选需要撤回的数据')
          return
      }
      this.$alertConfirmManager('提示', `确定要撤回${check_choose.length}条数据吗?`, '确定', '取消').then(res1 => {
          if (res1 === 'confirm') {
            const params = {
              ids: check_choose.map(item => item.id).join(','),
              status: 0
            }
            //status 为 0 则表示撤回
            outCoupon(params)
              .then(res => {
                  if (res && res.code === 0) {
                    this.$message.success('撤回成功')
                    this.getTableData()
                  }
              })
              .catch(res => {})
          } else {
            this.$message.info('取消操作')
          }
      })
    },
    // 发布
    outEventManager() {
      const check_choose = this.$refs.maintable.actionGetSelectedRow()
      if (!check_choose.length) {
          this.$message.info('请勾选需要发布的数据')
          return
      }
      this.$alertConfirmManager('提示', `确定要发布${check_choose.length}条数据吗?`, '确定', '取消').then(res1 => {
          if (res1 === 'confirm') {
            const params = {
              ids: check_choose.map(item => item.id).join(','),
              status: 1
            }
            // console.log(params)
            //发布
            outCoupon(params)
              .then(res => {
                  if (res && res.code === 0) {
                    this.$message.success('发布成功')
                    this.getTableData()
                  }
              })
              .catch(res => {})
          } else {
            this.$message.info('取消操作')
          }
      })
    },
    //置顶
    fixtopEventManager() {
      const check_choose = this.$refs.maintable.actionGetSelectedRow()
      if (!check_choose.length) {
          this.$message.info('请勾选需要置顶的数据')
          return
      }
      this.$alertConfirmManager('提示', `确定要置顶${check_choose.length}条数据吗?`, '确定', '取消').then(res1 => {
          if (res1 === 'confirm') {
            const params = {
              ids: check_choose.map(item => item.id).join(','),
              status: 1
            }
            //置顶
            fixtopCoupon(params)
              .then(res => {
                  if (res && res.code === 0) {
                    this.$message.success('置顶成功')
                    this.getTableData()
                  }
              })
              .catch(res => {})
          } else {
            this.$message.info('取消操作')
          }
      })
    },
    //取消置顶
    flextopEventManager() {
      const check_choose = this.$refs.maintable.actionGetSelectedRow()
      if (!check_choose.length) {
          this.$message.info('请勾选需要取消置顶的数据')
          return
      }
      this.$alertConfirmManager('提示', `确定要取消置顶${check_choose.length}条数据吗?`, '确定', '取消').then(res1 => {
          if (res1 === 'confirm') {
            const params = {
              ids: check_choose.map(item => item.id).join(','),
              status: 0
            }
            //置顶
            fixtopCoupon(params)
              .then(res => {
                  if (res && res.code === 0) {
                    this.$message.success('取消置顶成功')
                    this.getTableData()
                  }
              })
              .catch(res => {})
          } else {
            this.$message.info('取消操作')
          }
      })
    },
    //删除操作
    delEventManager() {
      const check_choose = this.$refs.maintable.actionGetSelectedRow()
      if (!check_choose.length) {
          this.$message.info('请勾选需要删除的数据')
          return
      }
      this.$alertConfirmManager('提示', `确定要删除${check_choose.length}条数据吗?`, '确定', '取消').then(res1 => {
          if (res1 === 'confirm') {
            // console.log(check_choose)
            const params = {
              id: check_choose.map(item => item.id).join(',')
            }
            // console.log(params)
            deleteCoupon(params)
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
    //修改
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
      else if(check_choose.length === 1){
        this.$refs.addDialogRef.open( check_choose[0], "edit")
      }
    },
  }
}

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