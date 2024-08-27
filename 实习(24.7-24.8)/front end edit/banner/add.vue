<template>
    <main-dialog
      ref="templateDialog"
      :transfer-title="editStatus | titleFilter"
      :transfer-width="500"
      :transfer-btn-list="[{ name: '保存' }, { name: '取消' }]"
      @actionEventManager="dialogActionEventManager"
    >
        <el-card class="box-card">
            <div style="width: 100%">
                <mainForm ref="queryForm" :model="queryInfo" :rules="formRules" label-width="120px">
                    <div class="form-cont">
                        <!-- *****编写弹窗内容-开始****** -->
                        <el-row>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="活动券数"
                                    transfer-form-prop-key="bannerNumber"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                            <el-col :span="15">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-title="优惠券"
                                    transfer-form-prop-key="name_coupon"
                                    :transfer-disabled="false"
                                    :transfer-fuzzy-dymic-query="{
                                        url: '/SeawinWebappBase/tCouponBanner/getCouponList.do',
                                        params: {
                                        },
                                        method: 'post'
                                    }"
                                    transfer-fuzzy-key = "name"
                                    :transfer-multiple = "true"
                                    @actionEventManager="setCoupon"
                                />
                            </el-col>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="运价"
                                    transfer-form-prop-key="bannerBackupTwo_show"
                                    :transfer-disabled="false"
                                    @actionEventManager="payEventManager"
                                />
                            </el-col>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="跳转路径"
                                    transfer-form-prop-key="bannerBackupThree"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                            <!-- 1.0上传图片文件url地址为双斜杠 -->
                            <el-col :span="15">
                                <main-group
                                    transfer-form-prop-key="banner_img"
                                    :transfer-line="1"
                                    transfer-title="banner图片"
                                >
                                    <base-upload
                                        ref="baseupload"
                                        :transfer-form-input="queryInfo"
                                        :transfer-upload-type="4"
                                        transfer-url="/SeawinWebappBase//upFile/uploadFileUrlLx.do"
                                        @actionEventManager="setImageUrl"
                                        @deleteTransfer="deleteUrl"
                                    />
                                </main-group>
                            </el-col>
                            <el-col :span="15">
                                <form-drop-list
                                    :transfer-form-input="queryInfo"
                                    transfer-title="活动类型"
                                    transfer-form-prop-key="activity_kind"
                                    :transfer-disabled="false"
                                    :transfer-fuzzy-fixed="['小程序', '门户','小程序特价','小程序直播']"
                                    @actionEventManager="setActivityKind"
                                />
                            </el-col>
                        </el-row>
                    </div>
                </mainForm>
            </div>
        </el-card>
        <add-dialog  
            ref="transpayDialogRef" 
            @actionChoose="getChooseResult"
        />
    </main-dialog>
</template>

<script>

import MainDialog from '@/components/CustomerWidge/MainWidget/mainDialog.vue'
import FormInput from '@/components/CustomerWidge/FormWidget/formInput.vue'
import FormDropList from '@/components/CustomerWidge/FormWidget/formDropList.vue'
import formUpload from '@/components/CustomerWidge/FormWidget/formUpload.vue'
import AddDialog from './transferpay.vue'
import { insertCoupon, editCoupon } from '@/api/smallProject.js'
import MainGroup from '@/components/CustomerWidge/MainWidget/mainGroup.vue'
import BaseUpload from '@/components/CustomerWidge/BaseWidget/baseUpload.vue'

export default{
    components: {
        MainDialog,
        FormDropList,
        formUpload,
        AddDialog,
        FormInput,
        MainGroup,
        BaseUpload
    },

    data() {
      return {
        editStatus: '',
        element_in:'',
        elytype:'',//管理新增时类型
        queryInfo: {
            name_coupon: '',//优惠券
            banner_img: '',//banner
            activity_kind: '',//活动类型
            bannerBackupTwo_show:'',//运价显示

            //需要上交的数据
            id:'', //id(唯一标识)
            bannerNumber:'', //banner数量
            couponId:'', //优惠券类型
            bannerBackupThree:'',//路径
            bannerBackupTwo:'',//先记录运价选择后指示，后指示传递情况；1-均未传入，3-仅传入运价，2-仅传入优惠券
            bannerImg:'',//图片连接
            bannerBackupOne:''//类型
        },
        formRules: {
            number_coupon: [{ required: true, message: '请输入券数', trigger: 'blur' },
                            { validator: this.validateCouponNumber, trigger: 'blur' }],
            activity_kind: [{ required: true, message: '请选择活动类型', trigger: 'change' }],
            banner_img: [{ required: true, message: '请上传图片', trigger: 'change' }]
        }
      }
    },

    filters: {
        titleFilter(val) {
            let title = ''
            switch (val) {
                case 'add':
                    title = '新增'
                break
                case 'edit':
                    title = '编辑'
                break
            }
            return title
        }
    },

    created() {
        this.initQueryInfo = JSON.parse(JSON.stringify(this.queryInfo))
    },

    mounted() {
    
    },

    methods:{
        // 弹窗回调
        dialogActionEventManager(method, item, cname, realItem) {
            console.log(method, item, cname, realItem)
            if (method === 'open') {
                // console.log('calheight', this.$refs.baseupload)
            } else if (method === 'buttonClick') {
                if (item.name === '保存') {
                    this.$refs['queryForm'].validate(valid => {
                        if (valid) {
                            const params_orign = {
                                ...this.queryInfo
                            }
                            //删除多余要素
                            delete params_orign['name_coupon']
                            delete params_orign['banner_img']
                            delete params_orign['activity_kind']
                            delete params_orign['bannerBackupTwo_show']
                            if(params_orign.couponId === ''){
                                params_orign.couponId = null
                            }
                            const params = {
                                keys: JSON.stringify(params_orign),
                                yunjiaType: ''
                            }
                            if(this.eltype !== '' && this.eltype !== undefined && this.eltype !== null){
                                params.yunjiaType = this.eltype
                            }
                            // console.log('params', params)
                            if (this.editStatus === 'edit') {
                                editCoupon(params).then(res => {
                                    if (res && res.code === 0) {
                                        this.$message.success('编辑成功')
                                        this.$refs.templateDialog.closeDelegate()
                                        this.$emit('actionEnd', 'ScucessAdd')
                                    }
                                    else if(res && res.code === 1){
                                        // this.$message.error('编辑失败')
                                        return false
                                    }
                                })
                            } else {
                                //新增
                                insertCoupon(params).then(res => {
                                    if (res && res.code === 0) {
                                        this.$message.success('新增成功')
                                        this.$refs.templateDialog.closeDelegate()
                                        this.$emit('actionEnd', 'ScucessAdd')
                                    }
                                    else if(res && res.code === 1){
                                        // this.$message.error('新增失败')
                                        return false
                                    }
                                })
                            }  
                        } else {
                            return false
                        }
                    })
                    
                } 
            } else if (method === 'calheight') {
                this.$nextTick(() => {
                    this.$refs.queryForm && this.$refs.queryForm.clearValidate()
                    // console.log('calheight', this.$refs.baseupload)
                    if(this.editStatus === 'add'){
                        this.$refs.baseupload.cleanFiles()
                        this.$refs.baseupload.setNone()
                    }
                    else if(this.editStatus === 'edit'){
                        this.$refs.baseupload.setUrl(this.queryInfo.bannerImg)
                    }
                })
            }
        },
        //open函数
        open( element, type = 'add') {
            this.editStatus = type
            this.element_in = element
            console.log('element_in', this.element_in)
            this.queryInfo = JSON.parse(JSON.stringify(this.initQueryInfo))
            if(type === 'edit'){
                // TODO: 载入显示/上传赋值--优惠券类型，123特殊对待
                //唯一标识
                this.queryInfo.id = element.id
                //数量
                this.queryInfo.bannerNumber = element.banner_number
                //活动类型
                this.queryInfo.activity_kind = element.banner_backup_one
                this.queryInfo.bannerBackupOne = element.banner_backup_one
                //图片信息
                this.queryInfo.bannerImg = element.banner_img
                this.queryInfo.banner_img = element.banner_img
                //路径信息
                this.queryInfo.bannerBackupThree = element.banner_backup_three
                if(element.banner_backup_two === '2')
                {
                    this.queryInfo.name_coupon = element.coupon_name
                    this.queryInfo.couponId = element.coupon_id
                }
                else if(element.banner_backup_two === '3'){
                    //运价显示
                    this.queryInfo.bannerBackupTwo_show = element.coupon_name
                    //运价记录
                    this.queryInfo.bannerBackupTwo = element.coupon_id
                }
            }
            this.$refs.templateDialog.open()
        },

        //运价弹窗
        payEventManager(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'focus') {
                this.$refs.transpayDialogRef.open()
            }
        },

        //验证券数范围
        validateCouponNumber(rule, value, callback) {
            //1.0支持优惠券达10位数，可认为无穷大
            if (value <= 0) {
                callback(new Error('活动券数必须大于0'));
            } else {
                callback();
            }
        },

        //获取内部抛出结果
        getChooseResult(method,item, itemId, elresult){
            if(method === 'chooseTransPayin'){
                console.log("获得成功",item)
                this.queryInfo.bannerBackupTwo_show = item
                this.queryInfo.bannerBackupTwo = itemId
                this.eltype = elresult
            }  
        },

        //选择优惠券类型
        setCoupon(method, item, cname, realitem) {
            console.log(method, item, cname,realitem)
            if (method === 'select') {
                this.queryInfo.couponId = ''
                if( realitem ){
                    this.queryInfo.couponId = realitem[0].item.code
                }
                for(let i = 1;i < realitem.length;i++){
                    this.queryInfo.couponId += ',' + realitem[i].item.code
                }
                console.log("check couponId",this.queryInfo.couponId)
            }
            else if (method === 'clear') {
                this.queryInfo.name_coupon = ''
                this.queryInfo.couponId = ''
            }
        },
        //选择活动类型
        setActivityKind(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.activity_kind = item.item
                this.queryInfo.bannerBackupOne = item.item
            }
            else if (method === 'clear') {
                this.queryInfo.activity_kind = ''
                this.queryInfo.bannerBackupOne = ''
            }
        },
        //获得上传图片地址
        setImageUrl(method, item, cname, realitem) {
            console.log(method, item,cname,realitem)
            if (method === 'upload') {
                this.queryInfo.bannerImg = item
                this.queryInfo.banner_img = item
                // console.log('Check', this.transferimagefilelist)
                // this.transferimagefilelist = []
                // this.transferimagefilelist.push(item)
                // console.log('Check', this.transferimagefilelist)
            }
            else if (method === 'clear') {
                this.queryInfo.bannerImg = ''
                this.queryInfo.banner_img = ''
                this.transferimagefilelist = []
            }
        },
        //删除已上传的url
        deleteUrl(method) {
            console.log(method)
            if(method === "delete")
            {
                this.queryInfo.bannerImg = ''
                this.queryInfo.banner_img = ''
            }
        }
    }
    



}

</script>

<style lang="scss" scoped>

</style>