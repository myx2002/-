<template>
  <!-- 详情 -->
  <view style="background-color:#F4F4F4">
    <tn-nav-bar>订单详情</tn-nav-bar>
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
						<text>提箱凭证</text> 
						<text class="ritEl"></text> 
					</view>
					<view class="line">
						<text>装箱地址</text> 
						<text class="ritEl">杭州萧山</text> 
					</view>
					<view class="line">
						<text>其他装箱地址</text> 
						<text class="ritEl"></text> 
					</view>
					<view class="line">
						<text>装箱要求</text> 
						<text class="ritEl">备注备注备注</text> 
					</view>
					<view class="line">
						<text>箱单下载</text> 
						<text class="ritEl"></text> 
					</view>
				</view>
			  </CardCont>
			  <CardCont title="装箱信息">
				  <view class="carInfo">
					<view class="line">
						<text>箱型</text>
						<text class="ritEl">40HQ</text>
					</view>
					<view class="line">
						<text>装箱时间</text>
						<text class="ritEl">2024.8.20 24:00</text>
					</view>
					<view class="line">
						<text>预报重量</text>
						<text class="ritEl">21000.000KGS</text>
					</view>
				  </view>
			  </CardCont>
			  <CardCont title="货运信息">
				<view class="carInfo">
				  <view class="line">
					<text>开港时间</text>
					<text class="ritEl">2024.8.20 10:00</text>
				  </view>
				  <view class="line">
					<text>截港时间</text>
					<text class="ritEl">2024.8.20 24:00</text>
				  </view>
				  <view class="line">
						<text>是否预提</text>
						<text class="ritEl">否</text>
				  </view>
				  <view class="line">
						<text>是否落箱</text>
						<text class="ritEl">否</text>
				  </view>
				</view>
			  </CardCont>
			  <CardCont title="其他信息">
				<view class="carInfo">
				  <view class="line">
					<text>操作姓名</text>
					<text class="ritEl">zzz</text>
				  </view>
				  <view class="line">
					<text>操作电话</text>
					<text class="ritEl">111xxxxxxxxxxx</text>
				  </view>
				  <view class="line">
					<text>业务姓名</text>
					<text class="ritEl">xxx</text>
				  </view>
				  <view class="line">
					<text>业务电话</text>
					<text class="ritEl">151xxxxxxxx</text>
				  </view>
				  <view class="line">
					<text>其他附件</text>
					<text class="ritEl"></text>
				  </view>
				</view>
			  </CardCont>
			</view>
		  </scroll-view>
		</view>
	</view>
	<view class="footCont">
		<view class="footButCan">
		  <text class="buttonCancel">异常上报</text>
		</view>
		<view class="footBut" v-if="type != -1">
		  <text class="button" v-if="type == 0">认领订单</text>
		  <text class="button" v-else-if="type == 1" @click="gotoDetailconfirm">上传箱封</text>
		  <text class="button" v-else-if="type == 2" @click="gotoDetailconfirm">上传箱封</text>
		  <text class="button" v-else-if="type == 3" @click="gotoDetailconfirm">到厂确认</text>
		  <text class="button" v-else-if="type == 4" @click="gotoDetailconfirm">装箱确认</text>
		  <text class="button" v-else-if="type == 5" @click="gotoDetailconfirm">离厂确认</text>
		  <text class="button" v-else-if="type == 6" @click="gotoDetailconfirm">进港确认</text>
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
			type:0
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
		gotoDetailconfirm(){
			uni.navigateTo({
				url: `/pages/detail/detailconfirm?type=${this.type}`,
			});
		}
	}
	
}
</script>

<style src="@/components/styles/index.scss" lang="scss"></style>
<style src="@/components/styles/common.scss" lang="scss"></style>
<style lang="scss" scoped>
@import url(@/components/styles/theme.css);	
</style>
