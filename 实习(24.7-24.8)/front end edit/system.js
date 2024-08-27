import request from '@/utils/request'

/** ******************************* 员工接口开始 ********************************/
export function employeeListByPage(data) {
  return request({
    url: '/SeawinWebappBase/employee/listByPage.do',
    method: 'post',
    data
  })
}
// 权限菜单
export function getPermissionUrl() {
  return request({
    url: '/SeawinWebappBase/sysPermission/getPermissionUrl.do',
    method: 'get'
  })
}

// 权限菜单
export function sysEmployeereateWxkf(params) {
  return request({
    url: '/hm/system/sysEmployee/createWxkf',
    method: 'get',
    params
  })
}

// 权限菜单已有员工
export function getEmployeeForPermission(params) {
  return request({
    url: '/SeawinWebappBase/sysPermission/getEmployeeForPermission.do',
    method: 'get',
    params
  })
}

// 修改菜单权限
export function updatePermission(data) {
  return request({
    url: '/SeawinWebappBase/sysPermission/updatePermission.do',
    method: 'post',
    data,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}
// 新增员工
export function insert(data) {
  return request({
    url: '/SeawinWebappBase/employee/insert.do',
    method: 'post',
    data,
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded;charset=utf-8'
    }
  })
}
// 修改员工
export function updateEmployee(data) {
  return request({
    url: '/SeawinWebappBase/employee/update.do',
    method: 'post',
    data,
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded;charset=utf-8'
    }
  })
}
// 删除员工
export function deleteEmployee(data) {
  return request({
    url: '/SeawinWebappBase/employee/delete.do',
    method: 'post',
    data
  })
}
// 获取白名单列表
export function getEmployeeList(data) {
  return request({
    url: '/SeawinWebappBase/redisController/listIdNameByName.do?name=employee',
    method: 'post',
    data
  })
}
/** =============================== 员工接口结束 ================================*/
/** =============================== 运价审批接口开始 ================================*/
// 获取白名单列表
export function SysSynchronizeConfigurationGetListByPage(data) {
  return request({
    url: '/SeawinWebappBase/SysSynchronizeConfiguration/getListByPage.do',
    method: 'post',
    data
  })
}
// 删除
export function SysSynchronizeConfigurationDelete(data) {
  return request({
    url: '/SeawinWebappBase/SysSynchronizeConfiguration/delete.do',
    method: 'post',
    data
  })
}
// 同步方Table数据
export function saleCustomerEmployeeListByNewBusiness(params) {
  return request({
    url: '/SeawinWebappBase/saleCustomerEmployee/listByNewBusiness.do',
    method: 'post',
    data: params
  })
}

// 新增----确定
export function SysSynchronizeConfigurationInsert(params) {
  return request({
    url: '/SeawinWebappBase/SysSynchronizeConfiguration/insert.do',
    method: 'post',
    data: params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}

// 修改----确定
export function SysSynchronizeConfigurationUpdate(params) {
  return request({
    url: '/SeawinWebappBase/SysSynchronizeConfiguration/update.do',
    method: 'post',
    data: params,
    headers: {
      'Content-Type': 'application/json;charset=utf-8'
    }
  })
}
/** =============================== 运价审批接口结束 ================================*/
/** =============================== 接口调用接口开始 ================================*/
export function esReportGetInterfaceinformationList(params) {
  return request({
    url: '/SeawinWebappBase/esReport/getInterfaceinformationList.do',
    method: 'post',
    data: params
  })
}
/** =============================== 接口调用接口结束 ================================*/
// OCR
export function yitaUploadFile(params) {
  return request({
    url: '/hm/track/yita/uploadFileByUrl',
    method: 'post',
    data: params
  })
}

//获取操作记录
export function getLogList(params) {
  return request({
    url: '/SeawinWebappBase/log/list.do',
    method: 'post',
    data: params
  })
}
//获取登录记录
export function getLoginLogList(params) {
  return request({
    url: '/SeawinWebappBase/loginlog/list.do',
    method: 'post',
    data: params
  })
}