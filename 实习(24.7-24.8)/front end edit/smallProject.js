// 小程序
import request from '@/utils/request'
import { MessageBox } from 'element-ui'
// 供应链管理
export function smallProjectGetUsersByProgram(start, length, params) {
  return request({
    url: '/hm/system/tMember/page',
    method: 'get',
    params: params
  })
}

// 修改用户类型

export function smallProjectUpdateUserType(memberId, userType) {
  var keys = { memberId: memberId, userType: userType }

  return request({
    url: '/SeawinWebappBase//tMember/updateUserType.do',
    method: 'get',
    data: keys,
    params: keys
  })
}

// 修改用户信息-备注

export function tMemberUpdate(data) {
  return request({
    url: '/hm/system/tMember/update',
    method: 'post',
    data: data,
    headers: {
      'Content-Type': 'application/json'
    }
  })
}

// 删除用户

export function smallProjectDeleteUserinfo(memberId) {
  var keys = { memberId: memberId }

  return request({
    url: '/SeawinWebappBase/tMember/deleteMember.do',
    method: 'get',
    data: keys,
    params: keys
  })
}

// 小程序
export function smalllistByNewBusiness(params) {
  var keys = {
    operatorLeader: '1',
    customerCode: params.customerCode,
    abbrForShow: params.abbrForShow,
    mobile: params.mobile
  }
  var keysStr = JSON.stringify(keys)
  var newParams = {
    start: (params.pageNumber - 1) * params.pageSize,
    length: params.pageSize,
    keys: keysStr
  }

  return request({
    url: '/SeawinWebappBase/saleCustomerEmployee/listByNewBusiness.do',
    method: 'post',
    data: newParams,
    params: newParams
  })
}

export function smalllistByNewBusinessV2(params) {
  var keys = {
    customerDefinition: '',
    customerServiceId: '',
    manInCharge: '',
    isQuit: '',
    customerStatusId: '',
    financialCode: '',
    salesmanId: '',
    isFinancialCode: '',
    companyBelonged: '',
    worthType: '',

    customerNameForShow: params.customerNameForShow,
    customerCode: params.customerCode,
    mobile: params.mobile
  }
  var keysStr = JSON.stringify(keys)
  var newParams = {
    start: (params.pageNumber - 1) * params.pageSize,
    length: params.pageSize,
    keys: keysStr
  }

  return request({
    url: '/hm/sale/saleCustomerEmployee/queryChargeBindPage',
    method: 'post',
    data: newParams
  })
}

// 审核
//  1已认证 2认证失败
export function smalllistByShenhe(id, status, remarks) {
  var params = { id: id, status: status, remarks: remarks }
  return request({
    url: '/SeawinWebappBase/tMember/customerCertificationAudit.do',
    method: 'post',
    data: params,
    params: params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 绑定用户
export function smalllistCustomerbinding(params) {
  var keys = JSON.stringify(params)
  var params1 = { keys: keys }
  return request({
    url: '/SeawinWebappBase/tMember/bindMiniProgramMothe.do',
    method: 'post',
    data: params1,
    params: params1,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 修改用户类型

export function smalllistUpdateUserType(memeberId, userType) {
  var params1 = { memberId: memeberId, userType: userType }
  return request({
    url: '/SeawinWebappBase/tMember/updateUserType.do',
    method: 'post',
    data: params1,
    params: params1,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 设置当前小程序用户为管理员

export function updateTmAdim(params) {
  return request({
    url: '/hm/system/tMember/updateTmAdim',
    method: 'get',
    params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// //////////////////////////////////////【小程序积分】//////////////////////////
// 获取用户积分

export function smalllisttScoreLogGetList(params) {
  var nParams = JSON.parse(JSON.stringify(params))
  var keys = {}
  if (nParams.name.length) {
    keys.name = nParams.name
  }
  if (nParams.phone.length) {
    keys.phone = nParams.phone
  }
  console.log(nParams)
  if (nParams.type) {
    keys.type = nParams.type
  }

  if (nParams.startDate.length) {
    keys.startDate = nParams.startDate
  }

  if (nParams.startEnd.length) {
    keys.startEnd = nParams.startEnd
  }

  var keyParams = JSON.stringify(keys)
  params.keys = keyParams
  return request({
    url: '/SeawinWebappBase/tScoreLog/getList.do',
    method: 'post',
    data: params,
    params: params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 快捷新增
export function saleCustomerInsertCustomerFast(data) {
  return request({
    url: '/SeawinWebappBase/saleCustomer/insertCustomerFast.do',
    method: 'post',
    data: data
    // headers: {
    //   'Content-Type': 'application/json;charset=utf-8'
    // }
  })
}

// 客户代码生成
export function saleCustomerCodeProgramFormat(data) {
  return request({
    url: '/hm/sale/saleCustomer/codeProgramFormat',
    method: 'post',
    data: data,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 评价列表

export function shipingEvaluationGetShipingEvaluationList(data) {
  return request({
    url: '/hm/shipping/shipingEvaluation/getShipingEvaluationList',
    method: 'post',
    data: data,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}
// // 解绑
// export function userUnbundling(params) {
//   return request({
//     url: '/SeawinWebappBase/user/unbundling.do',
//     method: 'get',
//     params: params
//   })
// }
// 解绑会员
export function tMemberUnbind(params) {
  return request({
    url: '/hm/system/tMember/unbind',
    method: 'get',
    params
  })
}

// 解绑-提交
export function userSwitchbinding(data) {
  return request({
    url: '/SeawinWebappBase/user/switchbinding.do',
    method: 'post',
    data: data
  })
}
// 拉黑---取消拉黑
export function tMemberUpdateShieldStatus(params) {
  return request({
    url: '/hm/system/tMember/updateShieldStatus',
    method: 'get',
    params: params
  })
}

// 积分核销 积分发放
export function tMemberWritePoints(data) {
  return request({
    url: '/hm/system/tMember/writePoints',
    method: 'post',
    data: data,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 分页获取蓝鳛登录日志
export function sysOperateLogPageLxLoginLog(params) {
  return request({
    url: '/hm/report/sysOperateLog/pageLxLoginLog',
    method: 'get',
    params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 绑定客户联系人
export function saleCustomerEmployeeBindCustomerEmployee(params) {
  return request({
    url: '/hm/sale/saleCustomerEmployee/bindCustomerEmployee',
    method: 'post',
    data: params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 分页查询蓝鳛操作日志列表
export function sysFunctionLogPageLxFunctionLog(params) {
  return request({
    url: '/hm/report/sysMenuLog/pageLxMenuLog',
    method: 'get',
    params
  })
}

// 删除积分
export function tScoreRuleDelete(params) {
  return request({
    url: 'SeawinWebappBase/tScoreRule/delete.do',
    method: 'post',
    data: params
  })
}

// 蓝鳛积分订单列表
export function lxScoreOrderOrderList(data) {
  return request({
    url: '/hm/system/lxScoreOrder/orderList',
    method: 'post',
    data: data,
    headers: {
      'Content-Type': '	application/json;charset=UTF-8'
    }
  })
}

// 填写发货信息
export function lxScoreOrderDeliveryOrder(params) {
  return request({
    url: '/hm/system/lxScoreOrder/deliveryOrder',
    method: 'post',
    data: params,
    headers: {
      'Content-Type': '	application/json;charset=UTF-8'
    }
  })
}

// 取消发货
export function lxScoreOrderCancellation(params) {
  return request({
    url: '/hm/system/lxScoreOrder/cancellation',
    method: 'post',
    data: params
  })
}

// 校验手机号是否存在
export function salePotentialCustomerCheckPhone(params) {
  return request({
    url: '/hm/sale/salePotentialCustomer/checkPhone',
    method: 'get',
    params
  })
}

// 修改会员信息
export function tMemberUpdateInfo(params) {
  return request({
    url: '/hm/system/tMember/updateInfo',
    method: 'post',
    data: params,
    headers: {
      'Content-Type': '	application/json;charset=UTF-8'
    }
  })
}

// 不再跟进
export function tMemberUpdateFollowStatus(params) {
  return request({
    url: '/hm/system/tMember/updateFollowStatus',
    method: 'post',
    data: params,
    headers: {
      'Content-Type': '	application/json;charset=UTF-8'
    }
  })
}

export function alertPromptManagerWithNogenjin(
  title,
  description,
  inputValue,
  confirmButtonText,
  cancelButtonText
) {
  if (!inputValue) {
    inputValue = ''
  }

  if (!confirmButtonText) {
    confirmButtonText = '确定'
  }

  if (!cancelButtonText) {
    cancelButtonText = '取消'
  }

  // 2. title
  var titleStr = title
  if (title === null) {
    titleStr = '提示'
  }

  var desStr = description
  if (description === null || description === undefined) {
    desStr = '当前无提示内容'
  }

  return new Promise((resolve, reject) => {
    MessageBox.prompt(desStr, titleStr, {
      confirmButtonText: confirmButtonText,
      cancelButtonText: cancelButtonText,
      inputValue: inputValue,
      inputValidator: value => {
        //非空验证
        if (!value) {
          return '请输入备注信息'
        }
      }
    })
      .then(({ value }) => {
        resolve({ type: true, info: value })
      })
      .catch(() => {
        resolve({ type: false, info: value })
      })
  })
}

//获取优惠券相关数据
export function getCouponList(data) {
  return request({
    url: '/SeawinWebappBase/tCouponBanner/getList.do',
    method: 'post',
    data
  })
}

//获取港口运输等相关数据
export function getTransportList(data) {
  return request({
    url: '/SeawinWebappBase/freightShippingQuotation/list.do',
    method: 'post',
    data
  })
}

//获取空运航线相关数据
export function getAirTransportList(data) {
  return request({
    url: '/SeawinWebappBase/freightAirQuotation/listByPage.do',
    method: 'post',
    data
  })
}

//新增优惠券数量
export function insertCoupon(data) {
  return request({
    url: '/SeawinWebappBase//tCouponBanner/insert.do',
    method: 'post',
    data
  })
}
//修改优惠券数量
export function editCoupon(data) {
  return request({
    url: '/SeawinWebappBase//tCouponBanner/update.do',
    method: 'post',
    data
  })
}
//删除制定优惠券
export function deleteCoupon(data) {
  return request({
    url: '/SeawinWebappBase//tCouponBanner/delete.do',
    method: 'post',
    data
  })
}
//发布特定的优惠券
export function outCoupon(data) {
  return request({
    url: '/SeawinWebappBase//tCouponBanner/updateStatus.do',
    method: 'post',
    data
  })
}
//置顶特定的优惠券
export function fixtopCoupon(data) {
  return request({
    url: '/SeawinWebappBase//tCouponBanner/updateTop.do',
    method: 'post',
    data
  })
}
//获取海报list
export function getPostList(data) {
  return request({
    url: '/SeawinWebappBase/postersAndShareImg/getList.do',
    method: 'post',
    data
  })
}
//删除指定海报
export function deletePost(data) {
  return request({
    url: '/SeawinWebappBase//postersAndShareImg/delete.do',
    method: 'post',
    data
  })
}
//新增海报
export function insertPost(data) {
  return request({
    url: '/SeawinWebappBase//postersAndShareImg/insert.do',
    method: 'post',
    data
  })
}
//修改海报
export function fixPost(data) {
  return request({
    url: '/SeawinWebappBase//postersAndShareImg/update.do',
    method: 'post',
    data
  })
}