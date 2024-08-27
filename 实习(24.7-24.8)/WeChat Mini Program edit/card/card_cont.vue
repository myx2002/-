<template>
	<!-- 公用卡片 - 框架 -->
	<view class="cardCont">
		<view class="topCont" @click="openHandle">
			<view class="">
				<text class="title">{{title}}</text>
				<text class="label" v-show="label">{{label}}</text>
			</view>
			<view class="openHandle">
				<image v-if="!isOpen" class="cardsIcon"  mode="aspectFit" src="@/static/image/add.png"></image>
				<image v-if="isOpen"  class="cardsIcon"  mode="aspectFit" src="@/static/image/minus.png"></image>
			</view>
		</view>
		<view class="content" :style="{height: (isOpen? height:0) +'px'}" ref="contEl">
			<view :id="elId" style="padding-top: 20rpx;">
				<slot></slot>
			</view>
		</view>
	</view>
	<!-- end -->
</template>

 <script>
export default {
	data() {
		const elId = `wl_${Math.ceil(Math.random() * 10e5).toString(36)}`
		return {
			isOpen: this.open,
			height: '',
			elId
		}
	},
	props: {
		// 列表标题
		title: {
			type: String,
			default: ''
		},
		//标题后面的说明文字
		label: {
			type: String,
			default: ''
		},
		// 是否展开
		open: {
			type: Boolean,
			default: false
		},
	},
	
	 updated() {
		this.$nextTick(() => {
			this.getCollapseHeight()
		});
	},
	mounted(){
		this.getCollapseHeight()
	},
	
	methods: {
		// 展开关闭操作
		openHandle(){
			this.isOpen = this.isOpen ? false : true;
		},
		// 获取内部容器的高度
		getCollapseHeight(type, index = 0) {
			const views = uni.createSelectorQuery().in(this)
			views
				.select(`#${this.elId}`)
				.fields({
					size: true
				}, data => {
					this.height = data.height
				})
				.exec()
		}
	},
}
</script>

<style lang="scss" scoped>
@import url(@/components/styles/theme.css);		
.cardCont{
	padding: 30rpx 40rpx;
	margin: 30rpx 0;
	border-radius: 20rpx;
	background: var(--neutral-color-white);
	.topCont{
		display: flex;
		justify-content: space-between;
		font-weight: 500;
		line-height: 60rpx;
		font-size: 32rpx;
		color: var(--neutral-color-main);
		align-items: center;
		// transition: 500ms;
		// transform: rotate(0deg);
		.title{
			font-weight: bold;
			font-size: 32rpx;
			color: var(--neutral-color-main);
		}
		.label{
			font-weight: 400;
			padding-left: 20rpx;
			font-size:var(--font-size-12);
			color:var(--neutral-color-font);
		}
		.redioActionCont{
			display: flex;
			align-items: center;
			.videoIcon{
				width: 64rpx;
				height: 64rpx;
			}
		}
		.cardsIcon{
			width: 48rpx;
			height: 48rpx;
		}
		.openHandle{
			display: flex;
			align-items: center;
		}
		.open{
			// transition: 500ms;
			transform: rotate(180deg);
		}
	}
	.content{
		overflow: hidden;
		transition-property: height, border-bottom-width;
		transition-duration: 0.3s;
		will-change: height;
	}
}
</style>
