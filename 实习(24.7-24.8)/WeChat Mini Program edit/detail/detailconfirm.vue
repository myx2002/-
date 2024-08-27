<template>
  <!-- 详情 -->
  <!-- TODO：接type修改标签，修改提示信息 -->
  <view style="background-color:#F4F4F4">
	<tn-nav-bar v-if="type == 1 || type == 2">上传箱封</tn-nav-bar>
	<tn-nav-bar v-else-if="type == 3">到厂确认</tn-nav-bar>
	<tn-nav-bar v-else-if="type == 4">装箱确认</tn-nav-bar>
	<tn-nav-bar v-else-if="type == 5">离厂确认</tn-nav-bar>
	<tn-nav-bar v-else-if="type == 6">进港确认</tn-nav-bar>
	<view :style="{marginTop:vuex_custom_bar_height + 'px',height:windowHeight + 'px'}">
		<view class="container">
		  <scroll-view scroll-y="true" class="swiperH">
			<view class="cont">
			  <CardCont title="基本信息" label = "(提单编号:1234)" :open="true">
				<view class="card">
					<view class="addrCont">
						<view class="startAddr">杭州萧山</view>
						<view class="midAddr">杭州萧山</view>
						<view class="endAddr">杭州萧山</view>
						<view class="">
							<text>当前状态：  </text>
							<text class="textfont" v-if="type == 0">待认领</text>
							<text class="textfont" v-else-if="type == 1">待上传箱封</text>
							<text class="textfont" v-else-if="type == 2">待上传箱封</text>
							<text class="textfont" v-else-if="type == 3">待到厂确认</text>
							<text class="textfont" v-else-if="type == 4">待装箱确认</text>
							<text class="textfont" v-else-if="type == 5">待离厂确认</text>
							<text class="textfont" v-else-if="type == 6">待进港确认</text>
						</view>
					</view>
				</view>
				<view class="carInfo">
					<view class="line"> 
						<text>船名航次</text> 
						<text class="ritEl">123456789</text> 
					</view>
					<view class="line">
						<text>提单号</text> 
						<text class="ritEl">987654321</text> 
					</view>
					<view class="line">
						<text>箱型</text> 
						<text class="ritEl">40HQ</text> 
					</view>
				</view>
				<view class="carInfo" v-if="type == 3 || type == 4 || type == 5">
					<view class="line">
						<text>装箱时间</text> 
						<text class="ritEl">2024.8.20 10:00:00</text> 
					</view>
					<view class="line">
						<text>装箱地址</text> 
						<text class="ritEl">杭州萧山</text> 
					</view>
					<view class="line">
						<text>装箱联系人</text> 
						<text class="ritEl">172xxxxxxxx</text> 
					</view>
				</view>
			  </CardCont>
			  <CardCont title="图片(定位)信息上传" :open="true" v-if="type != 6">
				<view class="upPicCont" v-if="type == 1 || type == 2">
					<view class="carema">
						<text class="tn-icon-camera-fill" style="margin-top:7rpx;"></text>
						<view class="title_bold"> 请拍照上传箱门照片</view>
					</view>
					<tn-image-upload :customBtn="true">
					    <view slot="addBtn" class="tn-image-upload__custom-btn">
							<view>请选择图片</view>
					    </view>
					</tn-image-upload>
					<!-- TODO:看一下输入框的使用 -->
					<view class="carInfo">
						<view class="line_other"> 
							<text style="margin-top:7rpx;">箱号：</text> 
							<tn-input type="text" border="true" placeholder="请上传箱号"/>
						</view>
						<view class="line_other">
							<text style="margin-top:7rpx;">箱型：</text> 
							<tn-input type="text" border="true" placeholder="请上传箱型"/>
						</view>
						<view class="line_other">
							<text style="margin-top:7rpx;">箱重：</text> 
							<tn-input type="text" border="true" placeholder="请上传箱重"/>
						</view>
					</view>
					<view class="carema">
						<text class="tn-icon-camera-fill" style="margin-top:7rpx;"></text>
						<view class="title_bold"> 请拍照上传箱门照片</view>
					</view>
					<tn-image-upload :customBtn="true">
					    <view slot="addBtn" class="tn-image-upload__custom-btn">
							<view>请选择图片</view>
					    </view>
					</tn-image-upload>
					<view class="carInfo">
						<view class="line_other"> 
							<text style="margin-top:7rpx;">封号：</text> 
							<tn-input type="text" border="true" placeholder="请上传封号"/>
						</view>
					</view>
				</view>
				<view class="upPicCont" v-if="type == 3 || type == 4 || type == 5">
					<view class="carema" v-if="type == 3">
						<text class="tn-icon-location-fill" style="margin-top:7rpx;"></text>
						<!-- TODO:当前getPosition函数为伪函数，需要解决真实位置获取及授权的问题 -->
						<view class="title_bold" @click="getPosition()" v-if="!current_position"> 获取定位【自动获取】</view>
						<view class="title_bold" @click="getPosition()" v-else> {{current_position}}</view>
					</view>
					<view class="carema">
						<text class="tn-icon-camera-fill" style="margin-top:7rpx;"></text>
						<view class="title_bold" v-if="type == 3"> 请拍照上传到厂照片</view>
						<view class="title_bold" v-else-if="type == 4"> 请拍照上传装箱照片</view>
						<view class="title_bold" v-else-if="type == 5">
							<view>要求必须包含:</view>
							<view style="font-weight:bold; margin-top:5rpx;">1.封箱照片   2.铅封特写</view>
						</view>
					</view>
					<tn-image-upload :customBtn="true">
					    <view slot="addBtn" class="tn-image-upload__custom-btn">
							<view>请选择图片</view>
					    </view>
					</tn-image-upload>
					<!-- 获取当前时间函数已实现 -->
					<view class="carInfo" @click="updateTime()">
						<view class="line_other" v-if="type == 3"> 
							<text class="tn-icon-time-fill" style="margin-top:7rpx;"></text>
							<text style="margin-top:7rpx; color:#000000" v-if="!current_time"> 到厂时间【点击获取】</text> 
							<view style="margin-top:7rpx; color:#000000" v-else> 到厂时间：{{current_time}}</view>
						</view>
						<view class="line_other" v-if="type == 4">
							<text class="tn-icon-time-fill" style="margin-top:7rpx;"></text>
							<text style="margin-top:7rpx; color:#000000" v-if="!current_time"> 开始装箱时间【点击获取】</text> 
							<view style="margin-top:7rpx; color:#000000" v-else> 开始装箱时间：{{current_time}}</view>
						</view>
						<view class="line_other" v-if="type == 5">
							<text class="tn-icon-time-fill" style="margin-top:7rpx;"></text>
							<text style="margin-top:7rpx; color:#000000" v-if="!current_time"> 完成装箱时间【点击获取】</text> 
							<view style="margin-top:7rpx; color:#000000" v-else> 完成装箱时间：{{current_time}}</view>
						</view>
					</view>
					<view class="carema" v-if="type == 5">
						<text class="tn-icon-inventory-fill" style="margin-top:7rpx;"></text>
						<view class="title_bold"> 工厂负责人签字</view>
						<!-- TODO:手写框待实现 -->
					</view>
				</view>
			  </CardCont>
			</view>
		  </scroll-view>
		</view>
	</view>
	<view class="footCont">
		<view class="footBut">
		  <text class="button">确定</text>
		</view>
		<view style="flex: 20"></view>
		<view class="footButCan">
		  <text class="buttonCancel">取消</text>
		</view>
	</view>
  </view>
  
</template>

<script>
import CardCont from '@/components/card/card_cont.vue';
export default{
	components: {
		CardCont
	},
	
	data(){
		return{
			type:0,
			current_time:'',
			current_position:''
		}
	},
	
	onReady() {
		console.log('页面初次显示')
		var windowHeight = uni.getSystemInfoSync().windowHeight;
		var safeArea = uni.getSystemInfoSync().safeArea.bottom - uni.getSystemInfoSync().safeArea.height;
		var vuex_custom_bar_height = this.vuex_custom_bar_height;
		var statusBarHeight = uni.getSystemInfoSync().statusBarHeight;
		this.windowHeight = windowHeight - vuex_custom_bar_height - safeArea - statusBarHeight;
		this.scrollViewHeight = this.windowHeight - 54;
	},
	
	onLoad(options){
		if (options.type) {
			this.type = options.type
		}
	},
	
	methods:{
		updateTime() {
		  const now = new Date();
		  this.current_time = now.toLocaleString();
		},
		
		getPosition(){
			this.current_position = "杭州萧山"
		}
	},
	
	mounted(){
	}
	
}
</script>

<style src="@/components/styles/index.scss" lang="scss"></style>
<style src="@/components/styles/common.scss" lang="scss"></style>
<style lang="scss" scoped>
@import url(@/components/styles/theme.css);	
.tn-image-upload__custom-btn {
	background-color: #E6E6E6;
	width: 100%;
	height: 180rpx;
	display: flex;
	align-items: center;
	justify-content: center;
	border-radius: 30rpx;
}
</style>