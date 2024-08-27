<template>
	<view style="background-color:#F4F4F4; height:100vh" >
		<view class="userContainer">
			<view class="topbackground">
				<view class="myInfo">
					<!-- TODO:动态获得头像图片 -->
					<image src="@/static/image/userIcon_sample.png" class="userIcon"></image>
					<text class="title">xxx</text>
					<text>司机编号：xxxxxxxxxx</text>
					<text>手机号：191xxxxxxxx</text>
				</view>
			</view>
			<view class="container">
				<view class="teskInfo">
					<view class="title">
						<text> -- 本月任务 -- </text>
					</view>
					<view class="info">
						<view class="cl" >
							<view class="num">75</view>
							<view class="title">订单总量</view>
						</view>
						<view class="cl">
							<view class="num">42</view>
							<view class="title">已完成订单数</view>
						</view>
						<view class="cl" >
							<view class="num">1345</view>
							<view class="title">运输总里程(km)</view>
						</view>
					</view>
				</view>
				<view class="navList">
					<view class="line" @click="gotoUserInfo">
						<text>车辆信息</text>
						<image class="icon" src="@/static/image/open_rit.png"></image>
					</view>
					<!-- <view class="line">
						<text>任务数据</text>
						<image class="icon" src="@static/iamge/open_rit.png"></image>
					</view> -->
					<view class="line" @click="gotoUserConfig">
						<text>系统设置</text>
						<image class="icon" src="@/static/image/open_rit.png"></image>
					</view>
				</view>
			</view>
		</view>
	</view>
</template>

<script>
	export default {
		components: {
		},
		
		data() {
			return {

			}
		},
		methods: {
			gotoUserConfig(){
				uni.navigateTo({
					url: '/pages/user/user_config',
				});
			},
			gotoUserInfo(){
				uni.navigateTo({
					url: '/pages/user/user_info',
				});
			}
		}
	}
</script>

<style src="@/components/styles/index_user.scss" lang="scss"></style>
<style src="@/components/styles/common.scss" lang="scss"></style>
<style lang="scss" scoped>
@import '@/tuniao-ui/tnSmart.scss';
@import url(@/components/styles/theme.css);	
.mine {
	max-height: 100vh;
}
.teskInfo{
	text-align: center;
	position: relative;
	z-index: 1;
	top: -180rpx;
	background: var(--neutral-color-white);
	border-radius: 16rpx;
	padding: 26rpx 48rpx;
	.title{
		padding-bottom: 20rpx;
		color: grey;
		font-weight: bold;
		font-size: 32rpx;
	}
	.info{
		display: flex;
		text-align: center;
		justify-content: space-between;
		line-height: 48rpx;
		color: var(--neutral-color-main);
		.cl{
			flex: 1;
			.num{
				line-height: 100rpx;
				font-weight: 600;
				font-size: var(--font-size-18);
			}
			.title{
				font-size: var(--font-size-13);
			}
		}
		
	}
}
</style>