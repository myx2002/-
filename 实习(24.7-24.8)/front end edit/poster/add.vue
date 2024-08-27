<template>
    <main-dialog
      ref="templateDialog"
      :transfer-title="enterType === 'add' ? '新增' : '修改'"
      :transfer-width="500"
      :transfer-btn-list="enterType === 'add'? [{ name: '保存' }, { name: '取消' }]:[{ name: '提交更改' }, { name: '取消' }]"
      @actionEventManager="dialogActionEventManager"
    >
        <p class = "red_color_content">
            新增时会默认填充参数,请勿随意修改参数,默认的参数是在海报的右上角<br>
            二维码X是二维码所在位置在海报最右边往左靠多少,单位是百分比<br>
            二维码Y是二维码所在位置在海报最底部往顶部靠多少,单位是百分比<br>
        </p>
        <el-card class="box-card">
            <div style="width: 100%">
                <mainForm ref="queryForm" :model="queryInfo" :rules="formRules" label-width="120px">
                    <div class="form-cont">
                        <!-- prop-key到时候再设置 -->
                        <el-row>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="二维码宽度"
                                    transfer-form-prop-key="width"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="二维码高度"
                                    transfer-form-prop-key="height"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="二维码Y"
                                    transfer-form-prop-key="y"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                            <el-col :span="15">
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="二维码X"
                                    transfer-form-prop-key="x"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                            <el-col :span="15">
                                <main-group
                                    transfer-form-prop-key="postersUrl"
                                    :transfer-line="1"
                                    transfer-title="海报"
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
                                <form-input
                                    :transfer-form-input="queryInfo"
                                    transfer-title="备注"
                                    transfer-form-prop-key="remark"
                                    :transfer-disabled="false"
                                />
                            </el-col>
                        </el-row>
                    </div>
                </mainForm>
            </div>
        </el-card>
    </main-dialog>
</template>

<script>
import MainDialog from '@/components/CustomerWidge/MainWidget/mainDialog.vue'
import FormInput from '@/components/CustomerWidge/FormWidget/formInput.vue'
import MainGroup from '@/components/CustomerWidge/MainWidget/mainGroup.vue'
import BaseUpload from '@/components/CustomerWidge/BaseWidget/baseUpload.vue'
import { insertPost,fixPost } from '@/api/smallProject.js'

export default {
    components: {
        MainDialog,
        FormInput,
        MainGroup,
        BaseUpload
    },

    data() {
        return {
            enterType: '',//进入类型
            queryInfo: {
                pasiId:'',//id
                width: '90',//二维码宽度
                height: '90',//二维码高度
                y: '90',//二维码Y
                x: '20',//二维码X
                postersUrl: '',//海报
                remark: ''//备注
            },
            formRules: {
                width: [{ validator: this.validateQRcodewidth, trigger: 'blur' }],
                height: [{ validator: this.validateQRcodeheight, trigger: 'blur' }],
                y: [{ validator: this.validateQRcodey, trigger: 'blur' }],
                x: [{ validator: this.validateQRcodex, trigger: 'blur' }],
                postersUrl: [{ validator: this.validateImg, trigger: 'blur' }]
            }
        }
    },

    methods: {
        //打开界面
        open(type, data = '') {
            this.enterType = type
            // console.log('type', data)
            if(type === 'edit')
            {
                this.queryInfo.width = data.pasiShareWidth
                this.queryInfo.height = data.pasiShareHeight
                this.queryInfo.y = data.pasiShareY
                this.queryInfo.x = data.pasiShareX
                this.queryInfo.postersUrl = data.pasiPosters
                this.queryInfo.remark = data.pasiStandbyOne
                this.queryInfo.pasiId = data.pasiId
            }
            this.$refs.templateDialog.open()
        },
        //验证图片格式
        validateImg(rule, value, callback) {
            console.log("rule",rule)
            console.log("value",value)
            if(!value){
                callback(new Error('请上传海报图片'))
            } else {
                let endIndex = value.lastIndexOf(".")
                let endType = value.substring(endIndex + 1)
                console.log("endType",endType) 
                if(endType === 'png' || endType === 'jpg' || endType === 'jpeg'||
                    endType === 'PNG' || endType === 'JPG' || endType === 'JPEG'||
                    endType === 'bmp' || endType === 'BMP')
                {
                    callback()
                }
                else{
                    callback(new Error('请上传正确格式的图片'))
                }
            }
        },
        //验证二维码宽度
        validateQRcodewidth(rule, value, callback) {
            if(!value){
                callback(new Error('请输入二维码宽度'))
            }
            else if (value <= 0) {
                callback(new Error('二维码宽度需要大于0'))
            } else {
                callback();
            }
        },
        //验证二维码高度
        validateQRcodeheight(rule, value, callback) {
            if(!value){
                callback(new Error('请输入二维码高度'))
            }
            else if (value <= 0) {
                callback(new Error('二维码高度需要大于0'))
            } else {
                callback();
            }
        },
        //验证二维码Y
        validateQRcodey(rule, value, callback) {
            if(!value){
                callback(new Error('请输入二维码Y值'))
            }
            else if (value < 0) {
                callback(new Error('二维码Y值需不小于0'))
            }
            else if (value > 100) {
                callback(new Error('二维码Y值需不大于100'))
            } else {
                callback();
            }
        },
        //验证二维码X
        validateQRcodex(rule, value, callback) {
            if(!value){
                callback(new Error('请输入二维码X值'))
            }
            else if (value < 0) {
                callback(new Error('二维码X值需不小于0'))
            }
            else if (value > 100) {
                callback(new Error('二维码X值需不大于100'))
            } else {
                callback();
            }
        },
        //获得上传图片地址
        setImageUrl(method, item, cname, realitem) {
            console.log(method, item,cname,realitem)
            if (method === 'upload') {
                this.queryInfo.postersUrl = item
                console.log(this.queryInfo.postersUrl)
            }
            else if (method === 'clear') {
                this.queryInfo.postersUrl = ''
            }
        },
        //删除已上传的url
        deleteUrl(method) {
            console.log(method)
            if(method === "delete")
            {
                this.queryInfo.postersUrl = ''
            }
        },
        //弹窗事件
        dialogActionEventManager(method, item, cname, realItem) {
            console.log(method, item, cname, realItem)
            if (method === 'open') {
                if (this.enterType === 'add') {
                    this.queryInfo = JSON.parse(JSON.stringify(this.init_queryInfo))
                }
            } 
            else if (method === 'buttonClick') {
                if (item.name === '保存' || item.name === '提交更改') {
                    this.$refs['queryForm'].validate(valid => {
                        if(valid){
                            const params_orign = {
                                ...this.queryInfo
                            }
                            if (this.enterType === 'edit') {
                                fixPost(params_orign).then(res => {
                                    if (res && res.code === 0) {
                                        this.$message.success('修改成功')
                                        this.$refs.templateDialog.closeDelegate()
                                        this.$emit('actionEnd', 'ScucessAdd')
                                    }
                                    else if(res && res.code === 1){
                                        return false
                                    }
                                })
                            } else {
                                //新增
                                insertPost(params_orign).then(res => {
                                    if (res && res.code === 0) {
                                        this.$message.success('新增成功')
                                        this.$refs.templateDialog.closeDelegate()
                                        this.$emit('actionEnd', 'ScucessAdd')
                                    }
                                    else if(res && res.code === 1){
                                        return false
                                    }
                                })
                            }  
                        } 
                        else {
                            return false
                        }
                    })     
                } 
            } 
            else if (method === 'calheight') {
                this.$nextTick(() => {
                    this.$refs.queryForm && this.$refs.queryForm.clearValidate()
                    if(this.enterType === 'add'){
                        this.$refs.baseupload.cleanFiles()
                        this.$refs.baseupload.setNone()
                    }
                    else if(this.enterType === 'edit'){
                        this.$refs.baseupload.setUrl(this.queryInfo.postersUrl)
                    }
                })
            }
        },
        
    },

    created() {
        this.init_queryInfo = JSON.parse(JSON.stringify(this.queryInfo))
    },

    mounted() {
        // Your mounted hook goes here
    }
}
</script>

<style lang="scss" scoped>
    .red_color_content{
        color: red;
    }
</style>