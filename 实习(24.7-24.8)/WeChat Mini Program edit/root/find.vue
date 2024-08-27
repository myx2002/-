<template>
	<view style="background-color:#F4F4F4">
		<!-- 顶部自定义导航 -->
		<tn-nav-bar :isBack="false">工作台</tn-nav-bar>

		<view :style="{marginTop:vuex_custom_bar_height + 'px',height:windowHeight + 'px'}">
			<!-- 1. 头部的搜索 -->
			<view style="width: 100%;" :style="{height:getHeight() + 'px'}" >
				<van-tabs swipeable :active="tabActive" @change="tabsActionEventManager">
					<div style='display: flex;width: 100%;flex-direction: column;justify-content:space-between;align-items: center;'>
						<div style="width: 100%;">
							<van-search :style="{'margin-top': vuex_custom_bar_height + 'px'}" :value="searchKey"
								placeholder="输入提单号进行搜索" @search="searchManager" @change="searchChangeManager">
							</van-search>
						</div>
						<div class="searchCont" v-if="tabId == 2">
							<view class="timeSearch" @click.stop="showOpen">
								<!-- TODO选择完成后的抛出 -->
								<uni-datetime-picker v-model="range" type="daterange" rangeSeparator=" 至 "
								@maskClick="recover" @change="recover_time"/>
								<view class="searchBut">
									<text class="button">筛选</text>
								</view>
							</view>
						</div>
					</div>
					<!-- TODO:函数未实现 -->
					<scroll-view  :style="{height:getHeight() - 60 + 'px'}" refresher-enabled="true" scroll-y="true"
						show-scrollbar="true" lower-threshold="200" enable-back-to-top="true"
						:refresher-triggered="refreshtriggered" @refresherrefresh="refresherrefreshManager"
						@refresherrestore="refresherrestoreManager" @scrolltolower="scrolltolowerManager"
						:id="scrollView" :scroll-top="scrollTop" @scroll="scroll">
						<!-- <van-tab title="订单认领">
							<Card :type = "1" :data="in_item"> </Card>
							<Card> </Card>
							<Card> </Card>
						</van-tab>
						<van-tab title="我的订单">
							<Card :type = "0" :data="in_item"> </Card>
							<Card :type = "2" :data="in_item"> </Card>
							<Card :type = "3"> </Card>
							<Card :type = "4"> </Card>
							<Card :type = "5"> </Card>
							<Card :type = "6"> </Card>
						</van-tab>
						<van-tab title="已完成订单">
							<Card :type = "-1" :data="in_item"> </Card>
							<Card :type = "-1"> </Card>
							<Card :type = "-1"> </Card>
						</van-tab> -->
						<van-tab title="订单认领">
						  <Card v-for="(item, index) in claimOrders" 
							:key="index" 
							v-bind:type="item.type ? item.type : 0"
							v-bind:data="item.data ? item.data : in_item">
						  </Card>
						</van-tab>
						<van-tab title="我的订单">
						  <Card v-for="(item, index) in out_myOrders" 
							:key="index"
							v-bind:type="item.type ? item.type : 0"
							v-bind:data="item.data ? item.data : in_item">
						  </Card>
						</van-tab>
						<van-tab title="已完成订单">
						  <Card v-for="(item, index) in completedOrders" 
							:key="index" 
							v-bind:type="item.type ? item.type : 0"
							v-bind:data="item.data ? item.data : in_item">
						  </Card>
						</van-tab>
					</scroll-view>
				</van-tabs>
				
			</view>
		</view>
	</view>
</template>

<script>
	import {memberHasLoginManager} from '@/api/login.js'
	import OrderCard from '@/pages/find/components/orderCard.vue'
	import Card from '@/components/card/card_index.vue'
	
	export default {
		name: 'FIND',
		components: {
			Card,
		},

		data() {
			return {
				type: 'normal',
				// 1. 判断是否登录 
				hasLogin: false,
				scrollView:"scrollView",
				windowHeight: 0,
				scrollViewHeight: 0,
				// 1. tabList
				// payStatus;
				tabList: ['订单认领', '我的订单', '已完成订单'],
				tabActive: '',
				tabId:'0',
				show:"true",
				// tableList;
				in_item:{
					TaskId:"1234",
					isoutput:0,
					startAddress:"杭州萧山",
					midAddress:"杭州萧山",
					endAddress:"杭州萧山",
					planDepartureTime:"2024.8.19 00:00:00 今天",
					alreadyPickUp:false,
					message:'',
					boxtype:"20HQ",
					boxweight:"8000KGS",
					dispatch:"派单：2024.8.18 00:00",
					day:"明天"
				},
				
				claimOrders: [],
				myOrders: [],
				completedOrders: [],
				
				out_myOrders: [], 
				itemsPerPage: 3, 
				currentPage_0: 0,
				currentPage_1: 0,
				currentPage_2: 0,
				scrollTop:0,
				old_scrollTop:0,
				  
				// 1.搜索key
				searchKey: '',

				// 搜索参数
				queryInfo: {
					pageNum: 1,
					pageSize: 10,
					deliveryId: '', // 运单号
					status: '',
				},
				refreshStatus: false, // 是否下拉过程
				refreshtriggered: true, // 复位状态 && 下拉状态
			}
		},

		onLoad() {
			console.log('页面加载1')

		},
		onShow() {
			console.log('页面显示')
		},
		onReady() {
			console.log('页面初次显示')
			var windowHeight = uni.getSystemInfoSync().windowHeight;
			var safeArea = uni.getSystemInfoSync().safeArea.bottom - uni.getSystemInfoSync().safeArea.height;
			var vuex_custom_bar_height = this.vuex_custom_bar_height;
			var statusBarHeight = uni.getSystemInfoSync().statusBarHeight;
			this.windowHeight = windowHeight - vuex_custom_bar_height - safeArea - statusBarHeight;
			this.scrollViewHeight = this.windowHeight - 54;
			
			// 订单认领
			this.claimOrders.push({ type: 1, data: this.in_item })
			this.claimOrders.push({ type: 0})
			this.claimOrders.push({ type: 0, data: this.in_item })
			
			// 我的订单
			this.myOrders.push({ type: 0, data: this.in_item })
			this.myOrders.push({ type: 2, data: this.in_item })
			this.myOrders.push({ type: 3, data: this.in_item })
			this.myOrders.push({ type: 4, data: this.in_item })
			this.myOrders.push({ type: 5, data: this.in_item })
			this.myOrders.push({ type: 6, data: this.in_item })
			this.myOrders.push({ type: 3, data: this.in_item })
			this.myOrders.push({ type: 4, data: this.in_item })
			this.myOrders.push({ type: 5, data: this.in_item })
			this.myOrders.push({ type: 6, data: this.in_item })
			
			// 已完成订单
			this.completedOrders.push({ type: -1, data: this.in_item })
			this.completedOrders.push({ type: -1, data: this.in_item })
			this.completedOrders.push({ type: -1, data: this.in_item })
			
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
		onShareAppMessage() {
			console.log('分享!')
		},
		onReachBottom() {
			console.log('下拉加载...')
		},
		onPageScroll() {
			console.log('页面滚动...')
		},
		onPullDownRefresh() {

		},
		
		mounted(){
			this.scrolltolowerManager()
		},

		methods: {
			recover_time(value){
				// console.log("change:",value)
				this.$emit('dateclose');
			},
			recover(value){
				// console.log("recover")
				this.$emit('dateclose');
			},
			showOpen(value){
				console.log("showOpen",value)
				// console.log("show",value._relatedInfo.anchorTargetText)
				if(value._relatedInfo.anchorTargetText == "确认"){
					this.$emit('dateclose');
					return
				}
				this.$emit('dateopen');
			},
			getHeight(){
				if(this.tabId != "2"){
					return this.scrollViewHeight
				}
				else if(this.tabId == "2"){
					// 64.2对映时间range选择框宽度
					return this.scrollViewHeight - 64.2
				}
			},
			//保留旧值
			scroll(event){
				// console.log("event scroll",event)
				this.old_scrollTop = event.detail.scrollTop
			},
			
			// 登录成功方法
			loginSuccessManager() {
				this.hasLogin = true;
			},

			// tab点击事件
			tabsActionEventManager(event) {
				console.log("event",event);
				this.tabActive = event.detail.title;
				this.tabId = event.detail.index;
				this.scrollTop = this.old_scrollTop
				this.$nextTick(()=>{
					this.scrollTop = 0
				});
				console.log("tabId",this.tabId)
				
				if(this.tabId == '0'){
					
				}
				else if(this.tabId == '1'){
					this.currentPage_1 = 0
					this.out_myOrders = []
				}
				else if(this,tabId == '2'){
					
				}
				this.scrolltolowerManager()
				
			},

			// 前往登录方法
			loginManager() {
				this.$emit('actionEventManager', 'login');
			},

		searchChangeManager(item){
			
		},

		
			// 下拉状态信息  自定义下拉刷新被触发	
			refresherrefreshManager() {
			
			},
			// 3.自定义下拉刷新被复位
			refresherrestoreManager() {
			
			},

			// 上啦
			//还有点小问题
			scrolltolowerManager() {
				if(this.tabId == "0"){
				}
				if(this.tabId == "1"){
				  const start = this.currentPage_1 * this.itemsPerPage;
				  const end = start + this.itemsPerPage;
				  if(end - this.itemsPerPage > this.myOrders.length){
					  return;
				  }
				  const moreOrders = this.myOrders.slice(start, end);
				  this.out_myOrders = [...this.out_myOrders, ...moreOrders];
				  this.currentPage_1++;
				  console.log(this.currentPage_1)
				}
				if(this.tabId == "2"){
				}
				
			},



			// 1. 返回到全部页，并刷新
			reloadAll() {
			
			}
		}
	}
</script>

<style src="@/components/styles/common.scss" lang="scss"></style>
<style lang="scss" scoped>
@import url(@/components/styles/theme.css);	
.content {
	height: 100%;
	background-color: red;
}

.bottom-button {
	width: 320rpx;
	height: 80rpx;
}
.searchCont{
	background-color: #fff;
	padding: 0rpx 30rpx 30rpx 30rpx;
	width: 100%;
	.searchBut{
		width: 180rpx;
		margin-left: 40rpx;
		.button{
			display: inline-block;
			padding:20rpx;
			color: var(--neutral-color-white);
			background-color: var(--essential-color-red);
			border-radius: 50rpx;
			text-align: center;
			box-sizing: border-box;
			width: 80%;
			font-size: var(--font-size-14);
		}
	}
	.timeSearch{
		display: flex;
		padding-top: 20rpx;
	}
}
</style>