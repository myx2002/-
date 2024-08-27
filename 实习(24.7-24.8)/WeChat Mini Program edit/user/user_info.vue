<template>
  <view style="background-color:#F4F4F4; height:100vh;">
    <tn-nav-bar>车辆信息</tn-nav-bar>
	<view class="carInfo">
		<image class="carImage" src="@/static/image/carImg_sample.jpg"></image>
		<view class="info">
			<view class="line">
				<text class="tit">车辆编号</text>
				<text>XYZ123456</text>
			</view>
			<view class="line">
				<text class="tit">车辆号牌</text>
				<text>浙A·123UI</text>
			</view>
			<view class="line">
				<text class="tit">车型</text>
				<text>大卡车A2</text>
			</view>
			<view class="line">
				<text class="tit">所属物流公司</text>
				<text>浙江xxxx物流集团</text>
			</view>
			<view class="line">
				<text class="tit">限额载重</text>
				<text>20吨</text>
			</view>
		</view>
	</view>
	
  </view>
</template>

<script>
	
</script>

<style lang="scss" scoped>
@import url(@/components/styles/theme.css);	
.carInfo{
	padding: 30rpx;
	.carImage{
		width: 100%;
		border-radius: 16rpx;
		margin-top: 180rpx;
	}
	.info{
		background: var(--neutral-color-white);
		border-radius: 20rpx;
		padding:40rpx;
		margin-top: 24rpx;
		font-size: var(--font-size-16);
		.line{
			display: flex;
			justify-content: space-between;
			line-height: 80rpx;
			color: var(--neutral-color-main);
			.tit{
				color: var(--neutral-color-font);
			}
		}
	}
}
</style>