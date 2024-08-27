<template>
    <main-dialog
        ref="templateDialog"
        :transfer-title="editStatus"
        :transfer-width="500"
        :transfer-btn-list="[{ name: '确定' }, { name: '取消' }]"
        @actionEventManager="dialogActionEventManager"
    >
        <el-card class="box-card">
            <div style="width: 100%">
                <mainForm ref="queryForm" :model="queryInfo" :rules="formRules" label-width="120px">
                    <div class="form-cont">
                        <el-row>
                            <form-drop-list
                                :transfer-form-input="queryInfo"
                                transfer-title="运价类型"
                                transfer-form-prop-key="transfer_pay"
                                :transfer-disabled="false"
                                :transfer-fuzzy-fixed="['海运运价', '空运运价']"
                                @actionEventManager="chooseTransferPay"
                            />
                        </el-row>
                    </div>
                </mainForm>
            </div>
        </el-card> 
        <add-dialog-sea  
            ref="seapayDialogRef" 
            @actionChoose="getChooseResult"
        />  
        <!-- <add-dialog-fly  ref="flypayDialogRef" /> -->
    </main-dialog>
</template>

<script>
import MainDialog from '@/components/CustomerWidge/MainWidget/mainDialog.vue'
import FormDropList from '@/components/CustomerWidge/FormWidget/formDropList.vue'
import AddDialogSea from './seatranspay.vue'
// import AddDialogFly from './flytranspay.vue'

export default {
    components: {
        MainDialog,
        FormDropList,
        AddDialogSea
    },


    data() {
        return {
            editStatus:"类型",
            queryInfo: {
                transfer_pay: '',
                transfer_payId: ''
            },
            formRules: {
                transfer_pay: [{ required: true, message: '请选择运价类型', trigger: 'change' }]
            }
        };
    },

    methods: {
        //open函数
        open() {
            this.$refs.templateDialog.open()
        },

        //选择运价类型类型
        chooseTransferPay(method, item, cname, realitem) {
            console.log(method, item)
            if (method === 'select') {
                this.queryInfo.transfer_pay = item.showValue
                this.queryInfo.transfer_payId = item.showKey
                // 0-海运，1-空运(注意是数字类型)
                // console.log(this.queryInfo.transfer_payId)
                // console.log(this.queryInfo.transfer_pay)
            }
            else if (method === 'clear') {
                this.queryInfo.transfer_pay = ''
                this.queryInfo.transfer_payId = ''
            }
        },

        getChooseResult(method,item, itemId, elresult){
            if(method === 'chooseTransPayin'){
                console.log("抛出成功",item)
                this.$emit('actionChoose','chooseTransPayin', item, itemId,elresult)
            }  
        },

        // 确定取消处理
        dialogActionEventManager(method, item, cname, realItem) {
            console.log(method, item, cname, realItem)
            if (method === 'open') {
                console.log('打开运价界面')
                // 消除验证
                this.$nextTick(() => {
                    this.$refs.queryForm && this.$refs.queryForm.clearValidate()
                })
            }
            else if (method === 'buttonClick') {
                if (item.name === '确定') {
                    this.$refs['queryForm'].validate(valid => {
                        if (valid) {
                            // console.log("111",this.queryInfo.transfer_payId)
                            if (this.queryInfo.transfer_payId === 0) {
                                // 关闭已有界面，打开新界面
                                this.$refs.templateDialog.closeDelegate()
                                this.$refs.seapayDialogRef.open("sea")
                            }
                            else if (this.queryInfo.transfer_payId === 1) {
                                this.$refs.templateDialog.closeDelegate()
                                this.$refs.seapayDialogRef.open("fly")
                            }
                        } else {
                            return false
                        }
                    })
                } else if (item.name === '取消') {
                    console.log('取消')
                }
            }
        },
    },

    created() {

    },

    mounted() {

    },
};
</script>



<style lang="scss" scoped>
</style>