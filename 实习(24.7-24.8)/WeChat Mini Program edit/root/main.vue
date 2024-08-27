<template>
	<view class="start-index">
		<!-- 1.首页 -->
		<!-- <view :style="{display: currentIndex === 0 ? '' : 'none'}">
			<scroll-view class="custom-tabbar-page" scroll-y enable-back-to-top @scrolltolower="tabbarPageScrollLower">
				<Home ref="home" @actionEventManager="homeEventManager"></Home>
			</scroll-view>
		</view> -->

		<!-- 2.订单列表 -->
		<view :style="{display: currentIndex === 0 ? '' : 'none'}">
			<scroll-view class="custom-tabbar-page" scroll-y enable-back-to-top @scrolltolower="tabbarPageScrollLower">
				<Find ref="find" @actionEventManager="discoveryEventManager" 
				@dateopen="changedisplay" @dateclose="changeshow"></Find>
			</scroll-view>
		</view>

		<!-- 3.个人中心 -->
		<view :style="{display: currentIndex === 1 ? '' : 'none'}">
			<scroll-view class="custom-tabbar-page" scroll-y enable-back-to-top @scrolltolower="tabbarPageScrollLower">
				<Center ref="center" @actionEventManager="mineEventManager"></Center>
			</scroll-view>
		</view>

		<!-- Tabbar -->
		<tn-tabbar v-model="currentIndex" :list="tabList" activeColor="#838383" inactiveColor="#AAAAAA"
			activeIconColor="#EF4F3F" :animation="true" :safeAreaInsetBottom="true" @change="switchTabbar"
		v-if="tabbarshow" iconSize = 50></tn-tabbar>

		<!-- 自动登录 -->
		<AutoLoginManager ref="autoLoginManager" @actionEventManager="loginSuccessManager" />
		
	</view>
</template>

<script>
	import Home from '@/pages/root/home.vue'
	import Find from '@/pages/root/find.vue'
	import Center from '@/pages/root/center.vue'
	import AutoLoginManager from '@/components/AutoLogin/AutoLoginManager.vue'
	export default {
		components: {
			Home,
			Find,
			Center,
			AutoLoginManager,
			
		},
		data() {
			return {
				tabList:[],
				// 底部tabbar菜单数据
				tabbarshow: true,
				tabbarList: [
					{
						title: '服务',
						activeIcon: 'rocket',
						inactiveIcon: 'cube'
					},

					{
						title: '我的',
						activeIcon: 'my-fill',
						inactiveIcon: 'my'
					}
				],
				// tabbar当前被选中的序号
				currentIndex: 0,
				// 自定义底栏对应页面的加载情况
				tabberPageLoadFlag: [],

				// 是否第一次加载
				firstLoadFind: false,
			}
		},
		onLoad(options) {

			const index = Number(options.index || 0)
			// 根据底部tabbar菜单列表设置对应页面的加载情况
			this.tabberPageLoadFlag = this.tabbarList.map((item, tabbar_index) => {
				return index === tabbar_index
			})
			this.switchTabbar(index)
			console.log('页面加载')
		},

		onShow() {
			console.log('页面显示')
		},
		onReady() {
			console.log('页面初次显示')
			this.tabList = this.tabbarList;
			if (this.$refs.autoLoginManager) {
				this.$refs.autoLoginManager.autoLoginManager();
			}
		},
		onHide() {
			console.log('页面隐藏')
		},
		onUnload() {
			console.log('页面卸载')
		},
		onBackPress() {
			console.log('页面返回...')
		},

		onReachBottom() {
			console.log('下拉加载...')
		},
		onPageScroll() {
			console.log('页面滚动...')
		},
		onPullDownRefresh() {
			console.log('上拉刷新...')
		},





		onShareAppMessage() {

			var shareInfo = '';
			var memberId = uni.getStorageSync('memberId')

			var url = '/pages/main?share=true&referralCode=' + memberId;

			console.log('我分享的url', url);
			return {
				title: '蓝鳛小程序',
				path: url,
				desc: '新蓝鳛，新启程。',
				imageUrl: 'https://lanmeng1.oss-cn-hangzhou.aliyuncs.com/supplychain/2023/02/28/802e3d31ca324f1db2d473ded9e51d7a.jpg'
			}
		},

		methods: {
			changedisplay(){
				console.log("111111")
				this.tabbarshow = false
			},
			changeshow(){
				console.log("222222")
				this.tabbarshow = true
			},
			// 切换导航
			switchTabbar(index) {
				// if (index === 1) {
				// 	if (this.$refs.find) {
				// 		this.$refs.find.loginSuccessManager();
				// 		// 第一次加载
				// 		if (this.firstLoadFind === false) {
				// 			this.$refs.find.tabActive = '全部';
				// 			this.firstLoadFind === true;
				// 		}
				// 	}
				// }
				this.tabList = this.tabbarList	
				this._switchTabbarPage(index)
			},


			// 瀑布流导航页面滚动到底部
			tabbarPageScrollLower(e) {
				// if (this.currentIndex === 2) {
				//   this.$refs.discovery.getRandomData && this.$refs.discovery.getRandomData()
				// }
			},

			// 切换导航页面
			_switchTabbarPage(index) {
				const selectPageFlag = this.tabberPageLoadFlag[index]
				if (selectPageFlag === undefined) {
					return
				}
				if (selectPageFlag === false) {
					this.tabberPageLoadFlag[index] = true
				}
				this.currentIndex = index
			},


			//////////////////////////////////////////////////////////////////////////////////////////   
			//  首页回调
			homeEventManager(method, item, cname, realItem) {
				if (method === 'login') {
					this.autoLoginManager();
				}
			},

			discoveryEventManager(method, item, cname, realItem) {
				if (method === 'login') {
					this.autoLoginManager();
				}
			},
			mineEventManager(method, item, cname, realItem) {
				if (method === 'login') {
					this.autoLoginManager();
				} else if (method === 'directToOrder') {
					this.switchTabbar(1);
				}
			},

			////////////////////////////////////////////////////////////////////////////////////////// 
			// 自动登录
			autoLoginManager() {
				this.$refs.autoLoginManager.loginManager()
			},

			// 登录成功方法
			loginSuccessManager(method, item, cname, realItem) {
				if (method === 'loginSuccess') {
					this.$refs.home.loginSuccessManager();
					this.$refs.find.loginSuccessManager();
					this.$refs.center.loginSuccessManager();
				}
			},
			
			////////////////////////////////////////////////////////////////////////////////////////// 
			// 支付成功，跳转到待接单订单页面，
			directToOrderAndChangeToWait(){
				this.switchTabbar(1);
				this.$refs.find.reloadAll();
			},
			////////////////////////////////////////首页////////////////////////////////////////////////// 
			homeDirectToOrderConfirm(){
				this.switchTabbar(0);
				this.$refs.home.jikuaidiManager();
			}
			

		}
	}
</script>

<style lang="scss" scoped>
</style>