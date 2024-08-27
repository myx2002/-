import Vue from 'vue'
import Router from 'vue-router'

Vue.use(Router)

import Layout from '@/layout'
import chartsRouter from './modules/charts' // 报表
import saleRouter from './modules/sale' // 合作方关系管理
import freightPrice from './modules/freightPrice' // 运价管理
import oceanExit from './modules/oceanExit' // 海运出口
import oceanBooking from './modules/oceanBooking.js' // 海运出口
import haiyunjinkouRouter from './modules/haiyunjinkou' // 海运进口
import logistics from './modules/logistics' // 物流
import contractManagement from './modules/contractManagement' // 合同管理
import workbench from './modules/workbench' // 工作台
import operatorBoard from './modules/operatorBoard' // 操作看板
import paperless from './modules/paperless' // 接单中心
import yichengjiaoyunjia from './modules/yichengjiaoyunjia'
import xunbaojia from './modules/xunbaojia'
import oceanImport from './modules/oceanImport'
import air from './modules/air' // 空运
import base from './modules/base' // 基础数据
import system from './modules/system'
import baobiao from './modules/baobiao'
import crm from './modules/crm' // CRM
import caiwu from './modules/caiwu'
import ediRizhi from './modules/ediRizhi'
import jituankehuManagement from './modules/jituankehuManagement' // 集团客户管理
import crossBorder from './modules/cross-border.js' // 跨境物流
import kehutuisongxinxiguanli from './modules/kehutuisongxinxiguanli.js' // 客户推送信息管理
import wenchuangduihuanshangcheng from './modules/wenchuangduihuanshangcheng.js' // 文创兑换商城

export const constantRoutes = [
  {
    path: '/login',
    component: () => import('@/pages/login/index'),
    hidden: true
  },
  {
    path: '/404',
    component: () => import('@/pages/404'),
    hidden: true
  },
  // 1. 空运询报价
  {
    // 我的报价
    path: '/airReply',
    name: 'airReply',
    component: () =>
      import(/* webpackChunkName: "xunbaojia" */ '@/pages/supplyChain/airtransport/customerBaojia/myFreightRateXunpan'),
    meta: { title: '我的报价', icon: 'dashboard', noCache: true },
    hidden: false
  },

  {
    // 询报价回复
    path: '/baojiahuifuWeb',
    name: 'baojiahuifuWeb',
    component: () => import('@/pages/supplyChain/oceanShipping/myBaojia/showModal/baojiahuifuWeb'),
    hidden: true
  },
  {
    path: '/form-generator',
    name: 'FormGenerator',
    title: '表单设计器',
    component: () =>
      import(/* webpackChunkName: "utils" */ '@/components/FormGenerator/pages-form-generator/Home.vue'),
    meta: { title: '表单设计器', icon: 'dashboard' },
    hidden: true
  },
  {
    // html案例
    path: '/case',
    name: 'case',
    component: () => import('@/pages/case/mainCase.vue'),
    hidden: true
  },

  {
    path: '/',
    component: Layout,
    redirect: '/dashboard',
    children: [
      {
        path: '/baobiaozhongxin',
        component: () => import(/* webpackChunkName: "seawin" */ '@/pages/baobiao/main.vue'),
        hidden: true
      },
      {
        path: 'dashboard',
        name: 'Dashboard',
        component: () => import('@/pages/dashboard/index'),
        meta: { title: '海盟管理系统2.0', icon: 'dashboard' }
      },
      {
        // 起运港附加费
        path: '/fujiafeiList',
        name: 'fujiafeiList',
        component: () => import('@/pages/supplyChain/oceanShipping/Fujiafei/fujiafeiList'),
        meta: { title: '起运港附加费', cache: true },
        hidden: true
      },

      {
        // 起运港附加费
        path: '/mudigangFujiafei',
        name: 'mudigangFujiafei',
        component: () =>
          import('@/pages/supplyChain/oceanShipping/Fujiafei/Mudigang/fujiafeiMudigangList'),
        meta: { title: '目的港附加费', cache: true },
        hidden: true
      },

      // { // 客拓统计
      //   path: '/tichengtongji',
      //   name: '客拓提成统计',
      //   component: () => import(/* webpackChunkName: "jiangzhi" */'@/pages/caiwu/baobiao/tichengtongji.vue'),
      //   meta: { title: '提成统计', icon: 'dashboard' },
      //   hidden: false
      // },
      {
        // 客拓统计
        path: '/ketuotichengtongji',
        name: 'ketuotichengtongji',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/ketuotichengtongji.vue'),
        meta: { title: '客拓提成统计', icon: 'dashboard' },
        hidden: false
      },
      {
        // 客服提成报表
        path: '/kefutichengbaobiao',
        name: 'kefutichengbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/kefutichengbaobiao/kefutichengbaobiao.vue'),
        meta: { title: '客服提成报表', icon: 'dashboard' },
        hidden: false
      },
      {
        // 报价客服提成报表
        path: '/baojiakefutichengbaobiao',
        name: 'baojiakefutichengbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/baojiakefutichengbaobiao.vue'),
        meta: { title: '客服提成报表', icon: 'dashboard' },
        hidden: false
      },
      {
        // 未计提查询报表
        path: '/weijitichaxunbaobiao',
        name: 'weijitichaxunbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/weijitichaxunbaobiao.vue'),
        meta: { title: '未计提查询报表', icon: 'dashboard' },
        hidden: false
      },
      {
        // 未收款报表
        path: '/weishoukuanbaobiao',
        name: 'weishoukuanbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/weishoukuanbaobiao.vue'),
        meta: { title: '未收款报表', icon: 'dashboard' },
        hidden: false
      },
      {
        // 商务提成统计
        path: '/shangwutichengtongji',
        name: 'shangwutichengtongji',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/shangwutichengtongji.vue'),
        meta: { title: '商务提成统计', icon: 'dashboard' },
        hidden: false
      },

      {
        // 风险金报表
        path: '/fengxianjinbaobiao',
        name: 'fengxianjinbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/fengxianjinbaobiao.vue'),
        meta: { title: '风险金报表', icon: 'dashboard' },
        hidden: false
      },
      {
        // 客拓风险金报表
        path: '/ketuofengxianjinbaobiao',
        name: 'ketuofengxianjinbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/ketuofengxianjinbaobiao.vue'),
        meta: { title: '客拓风险金报表', icon: 'dashboard' },
        hidden: false
      },

      {
        // 客服风险金报表
        path: '/kefufengxianjinbaobiao',
        name: 'kefufengxianjinbaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/kefu/kefufengxianjinbaobiao.vue'),
        meta: { title: '客服风险金报表', icon: 'dashboard' },
        hidden: false
      },
      {
        // 客服提成统计报表
        path: '/kefutichengtongji',
        name: 'kefutichengtongji',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/kefu/kefutichengtongji.vue'),
        meta: { title: '客服提成统计', icon: 'dashboard' },
        hidden: false
      },
      {
        // 客服提成统计报表
        path: '/kefutichengshujubaobiao',
        name: 'kefutichengshujubaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/kefu/kefutichengjichushujubaobiao.vue'),
        meta: { title: '客服提成基础数据报表', icon: 'dashboard' },
        hidden: false
      },

      {
        // 客服计提报表
        path: '/kefujitibaobiao',
        name: 'kefujitibaobiao',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/caiwu/baobiao/kefu/kefujitibaobiao.vue'),
        meta: { title: '客服计提报表', icon: 'dashboard' },
        hidden: false
      },

      {
        // 开票
        path: '/statementList',
        name: 'statementList',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/sale/accountstatement/statementList.vue'),
        meta: { title: '对账单', icon: 'dashboard', cache: true },
        hidden: false
      },
      {
        // 客户对账单(客服)
        path: '/kehuduizhangservice',
        name: 'kehuduizhangservice',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/sale/kehuDuizhangService/index.vue'),
        meta: { title: '客户对账单', icon: 'dashboard', cache: true },
        hidden: false
      },
      {
        // 价值客户
        path: '/jiazhikehushenqing',
        name: 'jiazhikehushenqing',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/jiazhikehu/jiazhikehu/index.vue'),
        meta: { title: '价值客户', icon: 'dashboard' },
        hidden: false
      },
      {
        // 价值客户
        path: '/kehusop',
        name: 'kehusop',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/jiazhikehu/kehuSOP/jiazhikehuList.vue'),
        meta: { title: '客户SOP', cache: true, icon: 'dashboard' },
        hidden: false
      },
      {
        // 测试
        path: '/test',
        name: 'test',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/components/CustomerWidge/WidgetTest/tableTest.vue'),
        meta: { title: '测试', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/enclosureList',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/logistics/enclosure/enclosureList'),
        hidden: true
      },

      {
        // 组织架构
        path: '/organizational',
        name: 'organizational',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/pages/organizational/index.vue'),
        meta: { title: '组织架构', icon: 'dashboard' },
        hidden: false
      },
      // 小程序 - 用户管理
      {
        path: '/xiaochengxuyonghu',
        name: 'xiaochengxuyonghu',
        component: () =>
          import(/* webpackChunkName: "xiaochengxu" */ '@/pages/smallProject/users/index.vue'),
        meta: { title: '小程序用户管理', cache: true, icon: 'dashboard' },
        hidden: false
      },
      //小程序 - banner管理
      {
        path: '/xiaochengxubanner',
        name: 'xiaochengxubanner',
        component: () =>
          import(/* webpackChunkName: "xiaochengxu" */ '@/pages/smallProject/banner/bannermanagement.vue'),
        meta: { title: '海盟管理系统', cache: true, icon: 'dashboard' },
        hidden: false
      },
      //小程序 - 海报管理
      {
        path: '/xiaochengxuposter',
        name: 'xiaochengxuposter',
        component: () =>
          import(/* webpackChunkName: "xiaochengxu" */ '@/pages/smallProject/poster/poster.vue'),
        meta: { title: '海盟管理系统', cache: true, icon: 'dashboard' },
        hidden: false
      },
      // 小程序 - 用户管理
      {
        path: '/xiaochengxumemberscore',
        name: 'xiaochengxumemberscore',
        component: () =>
          import(/* webpackChunkName: "xiaochengxu" */ '@/pages/smallProject/score/memberScore.vue'),
        meta: { title: '小程序用户积分管理', icon: 'dashboard' }
      },
      {
        path: '/xiaochengxupingjia',
        name: 'xiaochengxupingjia',
        component: () =>
          import(/* webpackChunkName: "xiaochengxu" */ '@/pages/smallProject/evaluate/indexmain.vue'),
        meta: { title: '小程序客户评价管理', icon: 'dashboard' }
      },
      {
        path: '/jifenshangpindingdan',
        name: 'jifenshangpindingdan',
        component: () =>
          import(/* webpackChunkName: "xiaochengxu" */ '@/pages/smallProject/ProductOrders/index.vue'),
        meta: { title: '积分商品订单', icon: 'dashboard' }
      },
      {
        path: 'xiaochengxucaozuorizhi',
        component: () =>
          import(/* webpackChunkName: "sale" */ '@/pages/smallProject/userOperate/useroperateList.vue'),
        name: 'xiaochengxucaozuorizhi',
        meta: { title: '用户操作日志', cache: true, showTag: true }
      },

      // 供应商评价列表
      {
        path: '/gongyingshangpingjialiebiao',
        name: 'gongyingshangpingjialiebiao',
        component: () =>
          import(/* webpackChunkName: "gongyshang" */ '@/pages/supplyChain/supplierEvaluate/main.vue'),
        meta: { title: '供应商评价列表', cache: true, icon: 'dashboard' },
        hidden: false
      },
      // 供应商评价详情
      {
        path: '/gongyingshangpingjiaxiangqing',
        name: 'gongyingshangpingjiaxiangqing',
        component: () =>
          import(/* webpackChunkName: "gongyshang" */ '@/pages/supplyChain/supplierEvaluate/detail.vue'),
        meta: { title: '供应商评价详情', icon: 'dashboard' },
        hidden: false
      },

      {
        path: '/colum',
        name: 'colum',
        component: () => import(/* webpackChunkName: "kongyun" */ '@/pages/template/colum.vue'),
        meta: { title: '表头sql 生成方法', icon: 'dashboard' },
        hidden: false
      },

      {
        path: '/testJson',
        name: 'testJson',
        component: () => import(/* webpackChunkName: "kongyun" */ '@/pages/template/testJson.vue'),
        meta: { title: '表头sql 生成方法', icon: 'dashboard' },
        hidden: false
      },

      {
        path: '/kefutichengjichushuju',
        name: 'kefutichengjichushuju',
        component: () =>
          import(/* webpackChunkName: "kongyun" */ '@/pages/caiwu/base/kefutichengjichushuju.vue'),
        meta: { title: '客服提成基础数据', icon: 'dashboard' },
        hidden: false
      },

      {
        path: '/fengxianjinbaobiaov2',
        name: 'fengxianjinbaobiaov2',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/baobiao/fengxianjinbaobiao.vue'),
        meta: { title: '风险金报表', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/yingshouhexiao',
        name: 'yingshouhexiao',
        component: () => import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/yingshou/list.vue'),
        meta: { title: '应收核销', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/yingfuduizhang',
        name: 'yingfuduizhang',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/yingfu/yingfuduizhang/list.vue'),
        meta: { title: '应付对账', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/chanpinqudaotongjipeizhi',
        name: 'chanpinqudaotongjipeizhi',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/baobiao/hangxianjiedan/chanpinqudaotongjipeizhi.vue'),
        meta: { title: '产品渠道统计配置', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/qaReport',
        name: 'QA',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/dashboard/comment/qaReport.vue'),
        meta: { title: 'QA', icon: 'dashboard' },
        hidden: true
      },

      {
        path: '/lishiyunjia',
        name: 'lishiyunjia',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/supplyChain/freightRate/freightRateHistoryList.vue'),
        meta: { title: '历史运价', icon: 'dashboard' },
        hidden: true
      },

      {
        path: '/lishiyunjia2',
        name: 'lishiyunjia2',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/supplyChain/oceanShipping/DeniseSearch/history/mainhistory.vue'),
        meta: { title: '历史运价', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/jikayunjia',
        name: 'jikayunjia',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/supplyChain/freightRateCar/main.vue'),
        meta: { title: '集卡运价', icon: 'dashboard' }
      },

      {
        path: '/formtest',
        name: 'FormGenerator-test',
        component: () =>
          import(/* webpackChunkName: "utils" */ '@/components/FormGenerator/formTest.vue'),
        meta: { title: '表单设计器', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/systemcaozuodialog',
        name: 'systemcaozuodialog',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/system/caozuodialog/caozuodialog.vue'),
        meta: { title: '海盟管理系统', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/systemdengludialog',
        name: 'systemdengludialog',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/system/dengludialog/dengludialog.vue'),
        meta: { title: '海盟管理系统', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/xiaochengxudenglurizhi',
        name: 'xiaochengxudenglurizhi',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/smallProject/loginNode/loginNode.vue'),
        meta: { title: '登录日志', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/kefutichengbiliweihu',
        name: 'kefutichengbiliweihu',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/baobiao/kefutichengbili/index.vue'),
        meta: { title: '客服提成比例', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/kaipiaoxinxi',
        name: 'kaipiaoxinxi',
        component: () => import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/fapiao/index.vue'),
        meta: { title: '发票', icon: 'dashboard' },
        hidden: true
      },

      {
        path: '/kefuniandudadiweihu',
        name: 'kefuniandudadiweihu',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/baobiao/kefutichengbili/kefuniandudadiweihu.vue'),
        meta: { title: '客服年度打底毛利', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/kehufeiyongmuban',
        name: 'kehufeiyongmuban',
        component: () =>
          import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/kaipiao/feiyongmuban/main.vue'),
        meta: { title: '客户费用模板', icon: 'dashboard' },
        hidden: true
      },
      {
        path: '/waizongfu',
        name: 'waizongfu',
        component: () => import(/* webpackChunkName: "seawin" */ '@/pages/lanxi/Zongfu/index.vue'),
        meta: { title: '外综服服务', icon: 'dashboard' },
        hidden: true
      },

      {
        // droplist
        path: '/drop-list',
        name: 'drop-list',
        component: () =>
          import(/* webpackChunkName: "jiangzhi" */ '@/components/CustomerWidge/WidgetTest/selectTest.vue'),
        meta: { title: '下拉封装', icon: 'dashboard' }
      },
      {
        path: '/outlinkiframe',
        name: 'outlinkiframe',
        component: () =>
          import(/* webpackChunkName: "baobiao" */ '@/components/Utils/outlinkIframe/index.vue'),
        meta: { title: '外部报表', icon: 'dashboard' }
      },
      {
        path: '/yingshoujiedianmingxishujubaobiao',
        name: 'yingshoujiedianmingxishujubaobiao',
        component: () => import(/* webpackChunkName: "seawin" */ '@/pages/caiwu/yingshoujiedianmingxishujubaobiao/index.vue'),
        meta: { title: '应收节点明细数据报表', icon: 'dashboard',cache: true, },
 
      },
     
    ]
  },

  {
    path: '/search',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/pages/oceanExit/detail/search/indexTable'),
    hidden: true
  },
  {
    path: '/template',
    component: () => import(/* webpackChunkName: "seawin" */ '@/pages/template/index'),
    hidden: true
  },
  // 客户业务海报
  {
    path: '/business-poster',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/pages/supplyChain/orderQuantityStatistics/components/BusinessPoster'),
    hidden: true
  },
  /*
   * 上海订单跟踪
   */
  {
    path: '/orderTrack',
    component: () => import(/* webpackChunkName: "seawin" */ '@/pages/oceanExit/orderTrack'),
    hidden: true
  },
  // 深圳港跟踪
  {
    path: '/port-trail',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/components/Utils/HgjPortTrack/index.vue'),
    hidden: true
  },
  // 飞驼船舶ifrme
  {
    path: '/ft-ship',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/components/Utils/FeituoShip/index.vue'),
    hidden: true
  },
  // 运小宝港区&船司跟踪
  {
    path: '/yxb-ocean-track',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/components/Utils/YxbOceanTrack/index.vue'),
    hidden: true
  },
  {
    path: '/wiki',
    component: () => import(/* webpackChunkName: "seawin" */ '@/pages/wiki/wikiList'),
    hidden: true
  },
  {
    path: '/wikiWrite',
    component: () => import(/* webpackChunkName: "seawin" */ '@/pages/wiki/wikiListWrite'),
    hidden: true
  },
  {
    path: '/maptest',
    component: () => import(/* webpackChunkName: "seawin" */ '@/pages/template/mapTest'),
    hidden: true
  },

  // 海运出口-预配舱单
  {
    path: '/yupeicangdan',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/components/oceanExit/yupeicangdan/index.vue'),
    hidden: true
  },

  // 海运出口-新跟踪看板
  {
    path: '/track-board',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/pages/oceanExit/components/TrackBoard/index.vue'),
    hidden: true
  },

  // 积分商城-嗨币排行榜
  {
    path: '/haibipaihang',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/pages/wenchuangduihuanshangcheng/components/haibipaihang.vue'),
    hidden: true
  },
  // 装满满3Difrome
  {
    path: '/zmm-iframe',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/pages/logistics/VisualPacking/ZmmIframe.vue'),
    hidden: true
  },
  // 车辆查询-GPS-路径
  {
    path: '/track-car-gps',
    component: () => import(/* webpackChunkName: "seawin" */ '@/components/Utils/track-car.vue'),
    hidden: true
  },
  {
    path: '/track-car-path',
    component: () => import(/* webpackChunkName: "seawin" */ '@/components/Utils/track-car.vue'),
    hidden: true
  },
  {
    path: '/track-car-h5',
    component: () => import(/* webpackChunkName: "seawin" */ '@/components/Utils/track-car-h5.vue'),
    hidden: true
  },
  // {
  //   path: '/tidanfenxiang',
  //   component: () =>
  //     import(/* webpackChunkName: "seawin" */ '@/components/Utils/tidanfenxiang.vue'),
  //   hidden: true
  // },
  {
    path: '/tidanfenxiang',
    component: () =>
      import(/* webpackChunkName: "seawin" */ '@/components/Utils/tidanfenxiang/index.vue'),
    hidden: true
  },

  oceanBooking,
  chartsRouter,
  saleRouter,
  freightPrice,
  oceanExit,
  haiyunjinkouRouter,
  logistics,
  contractManagement,
  workbench,
  paperless,
  yichengjiaoyunjia,
  operatorBoard,
  xunbaojia, // 询报价
  oceanImport, // 海运进口
  air, //  空运出口
  base,
  system, // 系统管理
  baobiao, // 报表
  crm, // crm
  caiwu,
  ediRizhi,
  jituankehuManagement,
  crossBorder,
  kehutuisongxinxiguanli,
  wenchuangduihuanshangcheng
]

const createRouter = () =>
  new Router({
    scrollBehavior: () => ({ y: 0 }),
    routes: constantRoutes
  })

const router = createRouter()
export default router
