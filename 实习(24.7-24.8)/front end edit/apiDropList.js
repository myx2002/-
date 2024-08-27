// 按钮点击去执行的接口

import networkAdapter from '@/utils/request'

// 获取textField的模糊查询的接口
export function networkAdapterWithDropList(url, paramsInfo, method, ContentType) {
  var params = JSON.parse(JSON.stringify(paramsInfo))
  // 过滤 2 个问号
  var keyArr = Object.keys(params)
  if (keyArr.length > 0) {
    keyArr.forEach(element => {
      var keySingleV = params[element]

      if (keySingleV === '??') {
        params[element] = ''
      }
    })
  }

  if (url === 'hm/sale/sysClientKnowledge/selectByBase') {
    if (params['name'] === '') {
      return new Promise((resolve, reject) => {
        resolve('none')
      })
    }
    return networkAdapter({
      url: url,
      method: 'post',
      data: params,
      params: params,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (url === 'hm/sale/valueCustomer/valueCustomerName') {
    return networkAdapter({
      url: url,
      method: 'post',
      data: params,
      params: params,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (
    url === '/SeawinWebappBase/redisController/fuzzyQueryIdNameByName.do' ||
    url === '/SeawinWebappBase/basedata/fuzzyQueryCommonSetMap.do' ||
    url === '/SeawinWebappBase/saleSupplier/listAllBookRoomName.do' ||
    url === '/SeawinWebappBase/basedata/fuzzyQueryPortMap.do'
  ) {
    return networkAdapter({
      url: url,
      method: 'get',
      data: params,
      params: params,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (url === '/hm/basedata/basedataPrintFormat/list') {
    return networkAdapter({
      url: url,
      method: 'get',
      data: params,
      params: params,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (
    url === '/SeawinWebappBase/basedataCommonSet/getMarketingItem.do' ||
    url === '/SeawinWebappBase/queryTemplate/getTemplateList.do'
  ) {
    params.keys && (params.keys['name'] = params.text)
    if (params.keys) {
      params.keys = JSON.stringify(params.keys)
    }
    if (method === 'get') {
      return networkAdapter({
        url: url,
        method: method,
        params: params,
        headers: {
          'Content-Type': ContentType
        }
      })
    } else {
      return networkAdapter({
        url: url,
        method: method,
        data: params,
        headers: {
          'Content-Type': ContentType
        }
      })
    }
  } else if (url === '/SeawinWebappBase/accountReceivableInvoice/listInvoiceType.do') {
    return networkAdapter({
      url: url,
      method: 'POST',
      data: params
    })
  } else if (url === '/hm/account/accountReceivableInvoice/listInvoiceType') {
    return networkAdapter({
      url: url,
      method: 'POST',
      data: params
    })
  } else if (url === '/SeawinWebappBase/basedataVessel/listByPage.do') {
    var keys = JSON.stringify(params)
    var tempParams = {
      start: 0,
      length: 100,
      keys: keys
    }
    return networkAdapter({
      url: url,
      method: method,
      data: tempParams,
      params: tempParams,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (
    url === '/hm/sale/saleCustomer/listCustomer' ||
    url === '/hm/sale/saleCustomer/listCustomerAndPotential'
  ) {
    return networkAdapter({
      url: url,
      method: 'GET',
      params: params,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (url === '/hm/sale/sysClientKnowledge/selectByBase') {
    return networkAdapter({
      url: url,
      method: 'POST',
      data: params,
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      }
    })
  } else if (url === '/SeawinWebappBase/saleSupplier/listByPageSelectToBusiness.do') {
    var newParams = {
      start: 0,
      length: 20,
      keys: JSON.stringify(params)
    }
    return networkAdapter({
      url: url,
      method: 'POST',
      data: newParams
    })
  } else if (url === 'hm/sale/saleCustomerEmployee/queryChargeForInvoice') {
    var keys = {
      customerNameForShow: params.customerNameForShow
    }

    params['keys'] = JSON.stringify(keys)
    return networkAdapter({
      url: url,
      method: 'POST',
      data: params
    })
  } else {
    if (method === 'get') {
      return networkAdapter({
        url: url,
        method: method,
        params: params,
        headers: {
          'Content-Type': ContentType
        }
      })
    } else {
      return networkAdapter({
        url: url,
        method: method,
        data: params,
        headers: {
          'Content-Type': ContentType
        }
      })
    }
  }
}

export function networkAdapterWithDropMainList(
  url,
  paramsInfo = {},
  method = 'post',
  ContentType = 'application/x-www-form-urlencoded'
) {
  return new Promise((resolve, reject) => {
    networkAdapterWithDropList(url, paramsInfo, method, ContentType).then(res => {
      if (!url) {
        return
      }

      if (url === 'hm/basedata/basedata/fuzzyQueryPort') {
        resolve(res.body)
      }
      if (url === '/hm/sale/saleCustomer/queryCustomerType') {
        resolve(res.body)
      } else if (url === '/hm/sale/tagRelation/relationConfig') {
        resolve(res.body)
      } else if (
        url === '/hm/sale/sysClientKnowledge/selectByBase' ||
        url === 'hm/sale/sysClientKnowledge/selectByBase'
      ) {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataPrintFormat/list') {
        resolve(res.body)
      } else if (url === '/hm/system/scoreTaskCategory/list') {
        resolve(res.body)
      } else if (url === '/hm/sale/tagConfig/getTagTypeEnum') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedata/fuzzyQuerySupplier') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleSupplier/fuzzyQuerySupplier') {
        resolve(res.body)
      } else if (
        url === '/hm/basedata/basedata/fuzzyQueryVesselParameter' ||
        url === 'hm/basedata/basedata/fuzzyQueryVesselParameter'
      ) {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataVesselParameter/page') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleCustomer/listCustomerAndPotential') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/singleCenter/getCharge.do') {
        if (res.data) {
          resolve(res.data)
        } else {
          resolve(res)
        }
      } else if (url === 'hm/system/sysEffictiveConfig/getButton') {
        resolve(res.body)
      } else if (
        url === '/SeawinWebappBase/basedata/fuzzyQueryCharge.do' ||
        url === 'SeawinWebappBase/basedata/fuzzyQueryCharge.do'
      ) {
        resolve(res.data)
        // } else if (url === 'hm/basedata/basedata/fuzzyQueryCommonSet') {
        //   resolve(res.body)
      } else if (url === 'hm/basedata/basedataCommonSet/searchSuperiorId') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleCustomer/listCustomer') {
        resolve(res.body)
      } else if (url === 'hm/crossborder/productCenter/getCompanyList') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/saleSupplier/listByPageSelectToBusiness.do') {
        resolve(res.aaData)
      } else if (url === 'hm/basedata/basedata/airQueryPort') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataChargeClassify/page') {
        resolve(res.body.list)
      } else if (
        url === '/hm/basedata/basedata/fuzzyQueryCommonSetFull' ||
        url === 'hm/basedata/basedata/fuzzyQueryCommonSetFull'
      ) {
        res.body.forEach(item => {
          item.showValue = item.code + '|' + item.cnName
          item.caozuoxishu = item.code + '|' + item.cnName
        })

        resolve(res.body)
      } else if (
        url === '/hm/basedata/basedata/fuzzyQueryCommonSet' ||
        url === 'hm/basedata/basedata/fuzzyQueryCommonSet'
      ) {
        res.body.forEach(item => {
          item.showValue = item.code + '|' + item.cnName
          item.caozuoxishu = item.code + '|' + item.cnName
        })

        resolve(res.body)
      } else if (
        url === '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_57'
      ) {
        console.log(res)
        const resurltList = []
        res.forEach(item => {
          console.log(item)
          const arrlist = item.split('|')
          const arrItem = arrlist[1]

          resurltList.push(arrItem)
        })
        resolve(resurltList)
      } else if (
        url ===
          '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_54_detail' ||
        url ===
          '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_53_detail'
      ) {
        res.forEach(item => {
          item.showValue = item.code + '|' + item.cn_name
        })
        resolve(res)
      } else if (
        url ===
        '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_51_detail'
      ) {
        res.forEach(item => {
          item.showValue = item.code + '|' + item.cn_name + '(' + item.en_name + ')'
        })
        resolve(res)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryPort.do') {
        res.data.forEach(item => {
          item.showValue = item.portCode + '|' + item.portName + '(' + item.portNameCn + ')'
        })
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/saleSupplier/listByKeys.do') {
        res.forEach(item => {
          item.showValue = item.customerCode + '|' + item.customerNameCn
        })
        resolve(res)
      } else if (
        url === '/SeawinWebappBase/employee/listAllEmployeeName.do' ||
        url === 'SeawinWebappBase/employee/listAllEmployeeName.do'
      ) {
        resolve(res)
      } else if (
        url ===
        '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_59_detail'
      ) {
        res.forEach(item => {
          item.showValue = item.code + '|' + item.cn_name
        })
        resolve(res)
      } else if (
        url ===
        'hm/system/sysOrganizationReport/list?keys=%7B%22type%22:%222%22,%22secondOrganizationId%22:30%7D'
      ) {
        resolve(res.body)
      } else if (url.includes('hm/system/sysOrganizationReport/list')) {
        resolve(res.body)
      } else if (
        url.includes('hm/system/sysOrganizationStructureReport/getSysEmployeeByReportId')
      ) {
        resolve(res.body)
      } else if (url.includes('hm/system/sysOrganizationStructureReport/getEmployeeByReportId')) {
        resolve(res.body)
      } else if (url.includes('/SeawinWebappBase/saleSupplier/getCopyCompanyGroupBy.do')) {
        const portKeysList = []
        const tempList = res
        const portKeys = Object.keys(tempList)
        portKeys.forEach(element => {
          const singleItem = {}
          singleItem.name = tempList[element]
          singleItem.key = element
          portKeysList.push(singleItem)
        })
        console.log(portKeysList)
        resolve(portKeysList)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryPort.do') {
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryContent.do') {
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/logisticsBill/getPort.do') {
        resolve(res.data)
      } else if (url === 'hm/basedata/basedataWharf/listByPortId') {
        resolve(res.body)
      } else if (url === 'hm/basedata/basedata/fuzzyQueryPort?flag=self') {
        res.body.unshift({ portId: 0, portName: 'DIR', portNameCn: 'DIR' })
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/product/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/channel/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/lcl/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/air/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/trailer/listPerson') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryCharge.do?flag=self') {
        res.data.forEach(item => {
          item.showValue = item.code + '|' + item.cnName
        })
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/basedata/queryBookingCompany.do') {
        res.forEach(item => {
          item.showValue = item.customerCode + '|' + item.customerNameCn
        })
        resolve(res)
      } else if (url === '/hm/crossborder/productCenter/getCompanyList') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleSupplier/saleSupplierListInfo') {
        resolve(res.body.records)
      } else if (url === '/hm/basedata/basedataLanShouWarehouse/page') {
        resolve(res.body.list)
      } else if (url === '/hm/basedata/basedataCommonSet/getByName') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataWarehouseAddr/page') {
        resolve(res.body.list)
      } else if (url === '/hm/basedata/basedataSailingDate/listServiceLineCode') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataRouteSchedule/list') {
        resolve(res.body)
      } else if (url === '/hm/shipping/shippingCrossBill/orderStatusList') {
        resolve(res.body)
      } else if (url === 'hm/sale/saleCustomerEmployee/queryChargeForInvoice') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/employee/listByPage.do') {
        resolve(res.aaData)
      } else if (url === '/hm/system/sysUser/listAccount') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/redisController/listIdObjectByname.do') {
        // 因公共方法不同传参返回不同结果集导致报错（无纸化箱型下拉）
        if (Array.isArray(res)) {
          res.forEach(item => {
            if (item.code && item.cn_name) {
              item.nodeValue = item.code + '|' + item.cn_name
            }
          })
        }
        console.log(667777)
        resolve(res)
      } else if (url === '/hm/basedata/basedataServiceLine/page') {
        resolve(res.body?.list || [])
      } else if (url === '/hm/account/accountReceivableInvoice/listInvoiceType') {
        resolve(res.body)
      } else if (url === '/hm/system/structure/getSecondCompany') {
        resolve(res.body || [])
      } else if (url === '/hm/system/sysUser/listAccount') {
        resolve(res.body)
      } else if (url === '/hm/crossborder/freightLogistics/getPort') {
        resolve(res.body)
      } else if (
        url === '/hm/crossborder/freightLogistics/getLogisticsFreithType' ||
        url === 'hm/crossborder/freightLogistics/getLogisticsFreithType'
      ) {
        resolve(res.body)
      } else if (
        url === 'hm/basedata/regions/queryFourAddress' ||
        url === '/hm/basedata/regions/queryFourAddress'
      ) {
        if (res.body) {
          res.body.forEach(element => {
            var mainAddress =
              element.provinceName +
              '/' +
              element.cityName +
              '/' +
              element.districtName +
              '/' +
              element.streetName +
              ' / ' +
              element.addressDetail
            element.mainAddress = mainAddress
          })
        }
        resolve(res.body)
      } else if (
        url === '/hm/basedata/regions/queryStreet' ||
        url === '/hm/basedata/regions/queryAllProvince' ||
        url === '/hm/basedata/regions/queryCity' ||
        url === '/hm/basedata/regions/queryDistrict'
      ) {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/redisController/listNameByname.do') {
        // 船务代理  、订舱方
        const resurltList = []
        res.forEach(item => {
          const arrlist = item.split('|')
          const arrItem = {
            nodeValue: arrlist[1],
            nodeKey: arrlist[0]
          }
          resurltList.push(arrItem)
        })
        resolve(resurltList)
      } else if (
        url.indexOf('/SeawinWebappBase/organization/listCompanyExceptLoginCompanyBySales.do') !== -1
      ) {
        resolve(['浙江海盟物流有限公司', '浙江海盟供应链管理有限公司上海分公司'])
      } else if (
        url ===
          '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_54_detail' ||
        url ===
          '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_53_detail'
      ) {
        res.forEach(item => {
          item.showValue = item.code + '|' + item.cn_name
        })
        resolve(res)
      } else if (
        url ===
        '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_51_detail'
      ) {
        res.forEach(item => {
          item.showValue = item.code + '|' + item.cn_name + '(' + item.en_name + ')'
        })
        resolve(res)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryPort.do') {
        res.data.forEach(item => {
          item.showValue = item.portCode + '|' + item.portName + '(' + item.portNameCn + ')'
        })
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/saleSupplier/listByKeys.do') {
        res.forEach(item => {
          item.showValue = item.customerCode + '|' + item.customerNameCn
        })
        resolve(res)
      } else if (
        url ===
        '/SeawinWebappBase/redisController/listIdObjectByname.do?name=basedataCommonSet_59_detail'
      ) {
        res.forEach(item => {
          item.showValue = item.code + '|' + item.cn_name
        })
        resolve(res)
      } else if (
        url ===
        'hm/system/sysOrganizationReport/list?keys=%7B%22type%22:%222%22,%22secondOrganizationId%22:30%7D'
      ) {
        resolve(res.body)
      } else if (
        url.includes('hm/system/sysOrganizationStructureReport/getSysEmployeeByReportId')
      ) {
        resolve(res.body)
      } else if (url.includes('hm/system/sysOrganizationStructureReport/getEmployeeByReportId')) {
        resolve(res.body)
      } else if (url.includes('/SeawinWebappBase/saleSupplier/getCopyCompanyGroupBy.do')) {
        const portKeysList = []
        const tempList = res
        const portKeys = Object.keys(tempList)
        portKeys.forEach(element => {
          const singleItem = {}
          singleItem.name = tempList[element]
          singleItem.key = element
          portKeysList.push(singleItem)
        })
        console.log(portKeysList)
        resolve(portKeysList)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryPort.do') {
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryContent.do') {
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/logisticsBill/getPort.do') {
        resolve(res.data)
      } else if (url === '/hm/logistics/logisticsMultiTransport/getPort.do') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleCustomer/listCustomerAndPotentialByQuote') {
        resolve(res.body)
      } else if (url === 'hm/basedata/basedataWharf/listByPortId') {
        resolve(res.body)
      } else if (url === 'hm/basedata/basedata/fuzzyQueryPort?flag=self') {
        res.body.unshift({ portId: 0, portName: 'DIR', portNameCn: 'DIR' })
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/product/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/channel/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/lcl/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/air/listPerson') {
        resolve(res.body)
      } else if (url === '/hm/report/business/manager/board/trailer/listPerson') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryCharge.do?flag=self') {
        res.data.forEach(item => {
          item.showValue = item.code + '|' + item.cnName
        })
        resolve(res.data)
      } else if (url === '/SeawinWebappBase/basedata/queryBookingCompany.do') {
        res.forEach(item => {
          item.showValue = item.customerCode + '|' + item.customerNameCn
        })
        resolve(res)
      } else if (url === '/hm/crossborder/productCenter/getCompanyList') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleSupplier/saleSupplierListInfo') {
        resolve(res.body.records)
      } else if (url === '/hm/basedata/basedataLanShouWarehouse/page') {
        resolve(res.body.list)
      } else if (url === '/hm/basedata/basedataCommonSet/getByName') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataWarehouseAddr/page') {
        resolve(res.body.list)
      } else if (url === '/hm/basedata/basedataSailingDate/listServiceLineCode') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataRouteSchedule/list') {
        resolve(res.body)
      } else if (url === '/hm/shipping/shippingCrossBill/orderStatusList') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/employee/listByPage.do') {
        resolve(res.aaData)
      } else if (url === '/hm/system/sysUser/listAccount') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/redisController/listIdObjectByname.do') {
        // 因公共方法不同传参返回不同结果集导致报错（无纸化箱型下拉）
        if (Array.isArray(res)) {
          res.forEach(item => {
            if (item.code && item.cn_name) {
              item.nodeValue = item.code + '|' + item.cn_name
            }
          })
        }
        console.log(667777)
        resolve(res)
      } else if (url === '/hm/basedata/basedataServiceLine/page') {
        resolve(res.body?.list || [])
      } else if (url === '/hm/account/accountReceivableInvoice/listInvoiceType') {
        resolve(res.body)
      } else if (url === '/hm/system/structure/getSecondCompany') {
        resolve(res.body || [])
      } else if (url === '/hm/system/sysUser/listAccount') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/redisController/listNameByname.do') {
        // 船务代理  、订舱方
        const resurltList = []
        res.forEach(item => {
          const arrlist = item.split('|')
          const arrItem = {
            nodeValue: arrlist[1],
            nodeKey: arrlist[0]
          }
          resurltList.push(arrItem)
        })

        resolve(resurltList)
      } else if (url === '/hm/system/sysRole/getList') {
        resolve(res.body || [])
      } else if (url === '/hm/system/sysResource/useListName') {
        resolve(res.body || [])
      } else if (url === 'hm/sale/saleCustomer/listCustomer') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataPort/getsStatsPortList') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataServiceLine/getsStatsLineList') {
        resolve(res.body)
      } else if (url === '/hm/basedata/basedataCommonSet/listBasedataCommonSet') {
        resolve(res.body)
      } else if (url === '/hm/sale/saleSupplier/getLogisticsBusinessType') {
        resolve(res.body)
      } else if (url === '/hm/system/sysOrganizationReport/getReportOrganizationType') {
        resolve(res.body)
      } else if (url === '/hm/sale/container/getAllPallet') {
        resolve(res.body)
      } else if (url === '/hm/logistics/logisticsMultiTransport/getAllTransportType') {
        resolve(res.body)
      } else if (url === 'hm/system/sysUser/listAccountByOauth') {
        resolve(res.body)
      } else if (url === '/SeawinWebappBase/basedataWharf/listByPage.do') {
        resolve(res.aaData)
      } else if (url === 'hm/report/operater/board/listPerson') {
        resolve(res.body)
      } else {
        resolve(res)
      }
    })
  })
}

// /// /// /// /// /// /// /// /// /// /// /// /// /// /droplist 的过滤 // /// /// /// /// /// /
// 封装的
export function dataSourceWithDymicSortManager(res, url) {
  console.log(55567, url, res)
  // 产品经理操作
  var tempArr = []
  if (
    url === '/hm/system/sysEmployee/getByDutiesCode' ||
    url === 'hm/system/sysEmployee/getByDutiesCode'
  ) {
    tempArr = res.body
  } else if (url === 'hm/sale/sysClientKnowledge/selectByBase') {
    // 客户sop的操作
    tempArr = res.body
  } else if (url === '/hm/track/logisticsTrack/getTrackAirStatusList') {
    tempArr = res.body
  } else if (url === '/hm/sale/saleSupplier/getLogisticsBusinessType') {
    tempArr = res.body
  } else if (url === '/SeawinWebappBase/basedata/fuzzyQueryContent.do') {
    tempArr = res.data
  } else if (url === '/SeawinWebappBase/queryTemplate/getTemplateList.do') {
    tempArr = res.data
  } else if (url === '/hm/basedata/basedataCommonSet/searchAllCountry') {
    tempArr = res.body
  } else if (url === '/SeawinWebappBase/basedataVessel/listByPage.do') {
    tempArr = res.aaData
  } else if (
    url === '/hm/basedata/basedata/fuzzyQueryVesselParameter' ||
    url === 'hm/basedata/basedata/fuzzyQueryVesselParameter'
  ) {
    // 获取船公司2.0
    tempArr = res.body
  } else if (url === '/hm/system/sysEmployee/listEmployee') {
    tempArr = res.body
  } else if (url === '/SeawinWebappBase/logisticsBill/getPort.do') {
    var portKeysList = []
    var tempList = res
    var portKeys = Object.keys(tempList)
    portKeys.forEach(element => {
      var singleItem = {}
      singleItem.name = tempList[element]
      singleItem.key = element
      portKeysList.push(singleItem)
    })
    tempArr = portKeysList
  } else if (url === '/SeawinWebappBase/saleSupplier/listByEmployeeId.do') {
    var keysItemList = []
    var keys = Object.keys(res)
    keys.forEach(element => {
      var singleItem = []
      singleItem = res[element]
      singleItem.key = element
      keysItemList.push(singleItem)
    })
    tempArr = keysItemList
  } else if (url === '/SeawinWebappBase/saleSupplier/listTruckSupplier.do') {
    const cheduiList = []
    const keys = Object.keys(res)
    keys.forEach(element => {
      var singleItem = []
      singleItem = res[element]
      singleItem.key = element
      cheduiList.push(singleItem)
    })
    tempArr = cheduiList
  } else if (url === '/SeawinWebappBase/basedata/getUnit.do') {
    if (res.data) {
      var info = Object.keys(res.data)
      tempArr = info
    }
  } else if (url === '/SeawinWebappBase/organization/listOrganizationExceptCompany.do') {
    var allkeys = Object.keys(res)
    var tempList = []
    allkeys.forEach(element => {
      tempList.push(res[element])
    })
    tempArr = tempList
  } else if (url === '/hm/basedata/basedata/listCharge') {
    tempArr = res.body
  } else if (url === '/hm/shipping/cargoSmart/edi/containerType') {
    const info = res.body
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = info[item]

      resultList.push(singleItem)
    })
    tempArr = resultList
  } else if (url === '/SeawinWebappBase/workflow/businessTypeList.do') {
    console.log(222, tempArr)
    // 站点
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      // var singleItem = info[item]

      // resultList.push(singleItem)
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        showname: singleItem,
        showkey: singleItemKey
      }
      resultList.push(aa)
    })
    tempArr = resultList
  } else if (url === '/SeawinWebappBase/logisticsBill/getPort.do') {
    // 站点
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      // var singleItem = info[item]

      // resultList.push(singleItem)
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        showname: singleItem,
        showkey: singleItemKey
      }
      resultList.push(aa)
    })
    tempArr = resultList
  } else if (url === '/hm/logistics/logisticsMultiTransport/getPort.do') {
    // 站点
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        locationText: singleItem,
        location: singleItemKey
      }
      resultList.push(aa)
    })
    tempArr = resultList
  } else if (url === '/SeawinWebappBase/user/getUserWithEmployee.do') {
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = { label: info[item], value: item }
      resultList.push(singleItem)
    })
    tempArr = resultList
    console.log(222, tempArr)
  } else if (url === '/SeawinWebappBase/workflow/businessTypeList.do') {
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        userName: singleItem,
        userId: singleItemKey
      }
      // resultList.push(singleItem)
      resultList.push(aa)
    })
    console.log(tempArr)
    tempArr = resultList
  } else if (url === '/hm/account/accountBillStatements/getBillStatesmentStatus') {
    tempArr = res.body
  } else if (url === '/hm/shipping/cargoSmart/edi/packageType') {
    const info = res.body
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = item

      resultList.push(singleItem)
    })
    tempArr = resultList
  } else if (url === '/SeawinWebappBase/user/getUserWithEmployee.do') {
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = info[item]

      resultList.push(singleItem)
    })
    tempArr = resultList
  } else if (
    url === '/hm/basedata/basedata/fuzzyQueryTruckSupplier' ||
    url === '/hm/sale/saleSupplier/fuzzyQueryTruck'
  ) {
    console.log(555, res)
    res.body.forEach(item => {
      item.value = item.customerCode + '|' + item.abbrCn
    })
    tempArr = res.body
    console.log(tempArr)
  } else if (url === '/hm/account/accountBillStatements/getBillStatesmentStatus') {
    tempArr = res.body
  } else if (url === '/hm/sale/saleSupplier/supplyContactQuiry') {
    tempArr = res.body
  } else if (url === '/SeawinWebappBase/redisController/listIdNameByName.do') {
    // 站点
    const info = res
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      // var singleItem = info[item]

      // resultList.push(singleItem)
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        showname: singleItem,
        showkey: singleItemKey
      }
      resultList.push(aa)
    })
    tempArr = resultList
  } else if (url === '/SeawinWebappBase/saleCustomer/getIndustryType.do') {
    console.log(res.data)
    const info = res.data
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        showname: singleItem,
        showkey: singleItemKey
      }

      resultList.push(aa)
    })
    tempArr = resultList
  } else if (url === '/SeawinWebappBase/saleCustomer/getSubIndustryType.do') {
    const info = res.data
    const keys = Object.keys(info)
    const resultList = []
    keys.forEach(item => {
      var singleItem = info[item]
      var singleItemKey = item
      const aa = {
        showname: singleItem,
        showkey: singleItemKey
      }

      resultList.push(aa)
    })

    tempArr = resultList
  } else if (url === 'hm/system/sysEmployee/list-by-key-name') {
    tempArr = res.body
  } else if (url === 'hm/sale/salePotentialCustomer/get_enums_customer') {
    tempArr = res.body
  } else if (url === 'hm/sale/salePotentialCustomer/get_enums_sale') {
    tempArr = res.body
  } else if (url.includes('SeawinWebappBase/saleCustomer/getIndustryTypeNew.do')) {
    tempArr = res.data
  } else if (url.includes('/SeawinWebappBase/saleCustomer/getSubIndustryTypeNew.do')) {
    tempArr = res.data
  } else if (url === 'hm/system/sysEmployee/fuzzyQueryByName') {
    tempArr = res.body
  } else if (url === '/hm/system/sysEmployee/fuzzyQueryByName') {
    res.body.forEach(element => {
      if (element.status === '禁用') {
        element.status1 = '离职'
      } else {
        element.status1 = ''
      }
    })

    tempArr = res.body
  } else if (url === '/hm/shipping/shippingCrossBill/orderStatusList') {
    const list = []
    Object.keys(res).forEach(key => {
      list.push({ nodeValue: key, nodeName: res[key] })
    })
    tempArr = list
  } else if (url === '/hm/system/sysEffictiveConfig/getModule') {
    tempArr = res.body
  } else if (url === '/hm/system/sysEffictiveConfig/getName') {
    tempArr = res.body
  } else if (url === '/hm/basedata/basedata/fuzzyQueryPort') {
    tempArr = res.body
  } else if (url === 'hm/shipping/shippingServiceNode/getType') {
    tempArr = res.body
  } else if (url === '/hm/sale/saleSupplierAddress/list') {
    tempArr = res.body
  } else if (url === 'hm/shipping/shippingConnectBill/listAllOverseaCompany') {
    tempArr = res.body
  } else if (url === '/hm/report/logisticsBoard/logisticsTrace') {
    tempArr = res.body
  } else if (url === '/hm/track/logisticsTrack/getTrackGoodsStatusList') {
    tempArr = res.body
  } else if (
    url === 'hm/crossborder/productServiceLine/listDischargingArea' ||
    url === 'hm/crossborder/productServiceLine/listServiceLineName'
  ) {
    tempArr = res.body
  } else if (url === '/hm/system/sysEmployee/page') {
    tempArr = res.body.list
  } else if (url === '/hm/basedata/basedataServiceLine/list') {
    tempArr = res.body
  } else if (url === 'hm/sale/qicc/queryCustomerNames') {
    tempArr = res.body || []
  } else if (url === '/hm/sale/tagConfigType/list') {
    tempArr = res.body || []
  } else if (url === 'hm/sale/qicc/queryCustomerNameSearch') {
    tempArr = res.body || []
  } else if (url === '/hm/report/sysDigitalData/listTitle') {
    tempArr = res.body || []
  } else if (url === '/hm/basedata/basedata/fuzzyQueryVesselParameter') {
    tempArr = res.body || []
  } else if (url === '/hm/system/structure/getWfCompanyEnum') {
    const list = []
    Object.keys(res.body).forEach((key) => {
      list.push({ code: key, name: res.body[key] })
    })
    tempArr = list
    console.log('tempArr', tempArr)
  }else if (url === '/hm/sale/saleSupplier/fuzzyQueryTruck') {
    tempArr = res.body || []
  } else if (url === '/SeawinWebappBase/tCouponBanner/getCouponList.do') {
    const list = []
    Object.keys(res).forEach((key) => {
      list.push({ code: key, name: res[key] })
    })
    tempArr = list
  }

  return tempArr
}
