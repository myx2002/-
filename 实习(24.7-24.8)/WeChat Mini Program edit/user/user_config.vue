<template>
  <view style="background-color:#F4F4F4; height:100vh;">
    <tn-nav-bar>系统设置</tn-nav-bar>
	<view class="carInfo">
		<view class="carImage"></view>
		<view class="systmSet">
			<view class="navList">
				<view class="line">
					<text>换绑手机</text>
					<image class="icon" src="@/static/image/open_rit.png"></image>
				</view>
				<view class="line">
					<text>换绑车辆</text>
					<image class="icon" src="@/static/image/open_rit.png"></image>
				</view>
				<view class="line">
					<text>修改密码</text>
					<image class="icon" src="@/static/image/open_rit.png"></image>
				</view>
				<view class="line">
					<text>消息通知设置</text>
					<image class="icon" src="@/static/image/open_rit.png"></image>
				</view>
			</view>
			<view class="back">
				<text>退出</text>
			</view>
		</view>
	</view>
 </view>
</template>

<script >
export default {
	data(){
		return {
			
		}
	},
	
	components:{
		
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
	
	methods:{
		
	}
}
</script>

<style src="@/components/styles/index_user.scss" lang="scss"></style>
<style src="@/components/styles/common.scss" lang="scss"></style>
<style lang="scss" scoped>
@import url(@/components/styles/theme.css);	
.carInfo{
	padding: 30rpx;
	.carImage{
		width: 100%;
		border-radius: 16rpx;
		margin-top: 180rpx;
	}
}
.systmSet{
	.navList{
		position: relative;
		background: var(--neutral-color-white);
		border-radius: 16rpx;
		padding: 0rpx 48rpx;
		margin: 40rpx 10rpx 80rpx 10rpx;
		font-size: var(--font-size-18);
		.line{
			line-height: 114rpx;
			border-bottom: solid 2rpx var(--neutral-color-cancel);
			display: flex;
			justify-content: space-between;
			align-items: center;
			.icon{
				width: 48rpx;
				height: 48rpx;
			}
		}
		.line:last-child{
			border: none;
		}
	}	
}
.back{
	position: relative;
	background: var(--essential-color-red);
	border-radius: 48rpx;
	padding: 0rpx 48rpx;
	margin: 80rpx 80rpx 40rpx 80rpx;
	
	text-align: center;
	line-height: 120rpx;
	font-size: var(--font-size-18);
	font-weight: bold;
	color: #FFFFFF;
	letter-spacing: 10rpx;
}
</style>