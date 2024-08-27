<template>
	<view class="template-index tn-safe-area-inset-bottom">

		<!-- navBar -->
		<tn-nav-bar fixed :isBack="false" :bottomShadow="false" backgroundColor="#3D7EFF80">
			<view class="tn-flex tn-flex-col-center tn-flex-row-left">
			
			</view>
		</tn-nav-bar>


		<!-- 背景动图 -->
		<view class="bg-contaniner tn-bg-blue">

		</view>

		<!-- 轮播图 -->
		<view class="tn-padding" :style="{paddingBottom:'0px', paddingTop: vuex_custom_bar_height + 'px'}">
			<HomeBanner />
		</view>


		<view class="tn-bg-white" style="border-radius: 40rpx 40rpx 0 0; ">
			<view style="padding-top: 25rpx;margin-top: 15rpx;">
				<view style="margin-left:20rpx;margin-right:20rpx; width:710rpx;align-items: center;" class="tn-flex tn-flex-row-between tn-bg-white about-shadow ">
					<view style="width:190rpx;display:flex;align-items: center;justify-content:center">
						<van-tag type="primary" >公告消息</van-tag>
						</view>
						
						<view style="width:100%">
							<van-notice-bar
								scrollable
								background="#fff"
								color="#000"
							  :text="notifyInfo"
							/>
						</view>
				</view>
				
			</view>
			
			<!-- 热图推荐-->
			<view style="margin-top: 20rpx;" class="tn-flex tn-margin-left tn-margin-right tn-margin-top ">
				
			</view>


	

			<!-- 胶囊banner -->
			<view class="tn-flex tn-flex-wrap tn-padding tn-shadow-blur" @click="directToExpressBooks()">
				<view class="" style="width: 100%;">
					<view class="image-piccapsule tn-shadow-blur"
						style=" background-image:url('https://iexpress-chire.oss-cn-beijing.aliyuncs.com/customerServer.jpeg');">
						<view class="image-capsule">
						</view>
					</view>
				</view>
			</view>

 <!-- 标题-->
    <view class="tn-flex tn-flex-row-between tn-flex-col-center tn-margin-top" @click="tn('')">
      <view class="justify-content-item tn-margin tn-text-bold tn-text-xl blue-title">
        关于我们
      </view>
      <view class="justify-content-item tn-margin-right tn-text-df tn-color-gray">
        <text class="tn-icon-right"></text>
      </view>
    </view>
    
    <!-- 功能入口-->
    <view class="tn-info__container tn-flex tn-flex-wrap tn-flex-col-center tn-flex-row-between tn-margin-left tn-margin-right">
      <block v-for="(item, index) in tuniaoData" :key="index">
        <view class="tn-info__item tn-flex tn-flex-direction-row tn-flex-col-center tn-flex-row-between tn-color-white tn-shadow-blur" :style="'background-color:'+ item.color +';'"  @click="directTo(item.url)">
          <view class="tn-info__item__left tn-flex tn-flex-direction-row tn-flex-col-center tn-flex-row-left">
            <!-- <view class="tn-info__item__left--icon tn-flex tn-flex-col-center tn-flex-row-center">
              <view :class="[`tn-icon-${item.icon}`]"></view>
            </view> -->
            <view class="tn-info__item__left__content">
              <view class="tn-info__item__left__content--title tn-text-bold" style="font-size: 38rpx;">{{ item.title }}</view>
              <view class="tn-info__item__left__content--data tn-padding-top-xs">
                {{ item.value }}
                <text class="tn-icon-right tn-padding-left-xs"></text>
              </view>
            </view>
          </view>
          <view class="tn-info__item__right">
            <view class="tn-info__item__right--icon">
              <view :class="[`tn-icon-${item.icon}`]"></view>
            </view>
          </view>
          <view class="tn-info__item__bottom">
            <view class='name tn-text-sm tn-color-gray' style="margin-left: -10rpx;">
              <text class="tn-icon-code tn-padding-right-xs" style="opacity: 0;"></text>
            </view>
          </view>
        </view>
      </block>
    </view>


	
			<view class='tn-tabbar-height'></view>

			<!-- 右侧按钮 -->
			<nav-index-button @actionEventManager = "couponActionClickManager"></nav-index-button>
		</view>
	</view>
</template>

<script>
	// 1.加入banner
	import HomeBanner from '@/components/HomeBanner/index.vue'
  import NavIndexButton from '@/libs/nav-index-button.vue'

	import wxUrlConfig from '@/common/config.js'

	import {
		authLogin
	} from '@/api/login.js'

	export default {
		name: 'Home',
		mixins: [],
		components: {
			HomeBanner,
			NavIndexButton,
		},
		data() {
			return {
				// 视图高度
				windowHeight: 0,
				scrollViewHeight: 0,
				// banner
				bannerList: [],
				notifyInfo:'',

				tuniaoData: [{
						title: '违禁物品',
						icon: 'image-text-fill',
						color: '#5177EE',
						value: '查看详情',
						url: '/pages/center/weijinpin'
					},
					{
						title: '常见问题',
						icon: 'image-fill',
						color: '#5F4FD9',
						value: '查看详情',
						url: '/pages/center/question/question'
					}
				],
			}
		},
		created() {
			this.interfacekdDeliveryBusinessList();
			// 获取通知
			this.interfacesystemNoticeList();
		},
		methods: {
			// 登录成功方法
			loginSuccessManager() {

			},

	
			directToExpressBooks() {
				
			},


			searchExpressManager() {
			
			},


			// 寄大件
			jidajianActionClickManager() {
			
			},


			// 寄快递
			jikuaidiManager() {
				
			},
			
			// 获取快递产品
			interfacekdDeliveryBusinessList(){
				
			},
			
			// 获取通知
			interfacesystemNoticeList(){
				this.notifyInfo = '123';
			},
			
			
			// 跳转到优惠券
			couponActionClickManager(){
				
			},
			
			directTo(url){
				
				
			}
		}
	}
</script>

<style lang="scss">
	.home_nav {
		.van-image {
			display: block !important;
			position: relative;
		}
	}
</style>



<style lang="scss" scoped>
	
	@import '@/tuniao-ui/tnSmart.scss';
	
	
	.template-index {
		max-height: 100vh;
	}

	.tn-tabbar-height {
		min-height: 100rpx;
		height: calc(120rpx + env(safe-area-inset-bottom) / 2);
	}



	/* 资讯主图 start*/
	.image-pic {
		// border: 1rpx solid #F8F7F8;
		background-size: cover;
		background-repeat: no-repeat;
		// background-attachment:fixed;
		background-position: top;
		border-radius: 10rpx;
	}

	/* 文字截取*/

	.blue-title::before {
		content: "";
		position: absolute;
		display: block;
		width: 80rpx;
		height: 26rpx;
		background: #269EFC;
		margin-top: 24rpx;
		margin-left: 75rpx;
		opacity: 0.3;
		z-index: -1;
		border-radius: 4rpx;
	}



	/* 热门图片 start*/
	.image-tuniao1 {
		padding: 164rpx 0rpx 162rpx 0rpx;
		font-size: 40rpx;
		font-weight: 300;
		position: relative;
	}

	.image-tuniao2 {
		padding: 74rpx 0rpx;
		font-size: 40rpx;
		font-weight: 300;
		position: relative;
	}

	.image-tuniao3 {
		padding: 90rpx 0rpx;
		font-size: 40rpx;
		font-weight: 300;
		position: relative;
	}

	.image-pic {
		background-size: cover;
		background-repeat: no-repeat;
		// background-attachment:fixed;
		background-position: top;
		border-radius: 10rpx;
	}

	/* 胶囊banner*/
	.image-capsule {
		padding: 100rpx 0rpx;
		font-size: 40rpx;
		font-weight: 300;
		position: relative;
	}

	.image-piccapsule {
		background-size: cover;
		background-repeat: no-repeat;
		// background-attachment:fixed;
		background-position: top;
		border-radius: 20rpx;
	}

	/* 工作区展示 start */
	.tn-info {

		&__container {
			margin-top: 10rpx;
			margin-bottom: 50rpx;
		}

		&__item {
			width: 47.7%;
			margin: 15rpx 0rpx 30rpx 0rpx;
			padding: 40rpx 30rpx;
			border-radius: 10rpx;


			position: relative;
			z-index: 1;

			&::after {
				content: " ";
				position: absolute;
				z-index: -1;
				width: 100%;
				height: 100%;
				left: 0;
				bottom: 0;
				border-radius: inherit;
				opacity: 1;
				transform: scale(1, 1);
				background-size: 100% 100%;
				background-image: url(https://resource.tuniaokj.com/images/cool_bg_image/2.png);
			}

			&__left {

				&--icon {
					width: 80rpx;
					height: 80rpx;
					border-radius: 50%;
					font-size: 40rpx;
					margin-right: 20rpx;
					position: relative;
					z-index: 1;

					&::after {
						content: " ";
						position: absolute;
						z-index: -1;
						width: 100%;
						height: 100%;
						left: 0;
						bottom: 0;
						border-radius: inherit;
						opacity: 1;
						transform: scale(1, 1);
						background-size: 100% 100%;
						background-image: url(https://resource.tuniaokj.com/images/cool_bg_image/icon_bg5.png);
					}
				}

				&__content {
					font-size: 25rpx;

					&--data {
						color: rgba(255, 255, 255, 0.5);
						margin-top: 5rpx;
						// font-weight: bold;
					}
				}
			}

			&__right {
				&--icon {
					position: absolute;
					right: 0rpx;
					top: 50rpx;
					font-size: 100rpx;
					width: 108rpx;
					height: 108rpx;
					text-align: center;
					line-height: 60rpx;
					opacity: 0.15;
				}
			}

			&__bottom {
				box-shadow: 0rpx 0rpx 30rpx 0rpx rgba(0, 0, 0, 0.12);
				border-radius: 0 0 10rpx 10rpx;
				position: absolute;
				width: 85%;
				line-height: 15rpx;
				left: 50%;
				bottom: -15rpx;
				transform: translateX(-50%);
				z-index: -1;
				text-align: center;
			}
		}
	}

	/* 工作区展示 end */










	/* 移动背景部分 start*/
	.bg-contaniner {
		position: fixed;
		top: -0rpx;
		left: -300rpx;
		--text-color: hsl(0 95% 60%);
		--bg-color: hsl(0 0% 100%);
		--bg-size: 200px;
		height: 100%;
		display: grid;
		place-items: center;
		place-content: center;
		overflow: hidden;
		background-color: var(--bg-color);
		z-index: -1;
	}

	.bg-contaniner::before {
		--size: 150vmax;
		content: "";
		inline-size: var(--size);
		block-size: var(--size);
		background-image: url("https://iexpress-chire.oss-cn-beijing.aliyuncs.com/background.png");
		// background-image: url("https://lanmeng1.oss-cn-hangzhou.aliyuncs.com/test/2023/02/15/6b00d305d6c94a3e8ae2db13646d237b.png");
		background-size: var(--bg-size);
		background-repeat: repeat;
		transform: rotate(45deg);
		opacity: 0.25;
		animation: bg 6s linear infinite;
	}

	@media (prefers-reduced-motion: reduce) {
		.bg-contaniner::before {
			animation-duration: 0s;
		}
	}

	@keyframes bg {
		to {
			background-position: 0 calc(var(--bg-size) * -1);
		}
	}

	/* 移动背景部分 end*/
</style>