<template>
	<!-- 公用卡片 -->
	<view class="card" @click="gotoDetail(item)">
		<view class="topCont"> 
			<text>提单号：{{item.TaskId}}</text> 
			<text v-if="item.isoutput === 1" class="label">出口</text>
			<text v-else-if="item.isoutput === 0" class="label">进口</text>
		</view>
		<view class="otherinfo">
			<text class="space">{{item.boxtype}}</text> 
			<text class="space">{{item.boxweight}}</text>
			<text v-if=" type === 0">{{item.dispatch}}</text>
			<text v-else>{{item.day}}</text>
		</view>
		<view class="addrCont">
			<view class="startAddr">{{item.startAddress}}</view>
			<view class="midAddr">{{item.midAddress}}</view>
			<view class="endAddr">{{item.endAddress}}</view>
			<view class="topspace">
				<text>备注：{{item.message}}</text>
			</view>
		</view>
		<view class="botCont">
			<view class="timeCont">
				<view class="tit"> 
					<text>做箱时间</text> 
				</view>
				<view class="time"> 
					<text>{{item.planDepartureTime}}</text> 
				</view>
			</view>
			<!-- 先通过type来区别类型-->
			<view class="ButCont" v-if="type === 0"> 
				<text class="butRed">未认领</text>
			</view>
			<view class="ButCont" v-else-if="type === 1">
				<text class="butRed butDis">已认领</text>
			</view>
			<view class="ButCont" v-else-if="type === 2">
				<text class="butRed">待上传箱封</text>
			</view>
			<view class="ButCont" v-else-if="type === 3">
				<text class="butRed">待到厂</text>
			</view>
			<view class="ButCont" v-else-if="type === 4">
				<text class="butRed">待装箱</text>
			</view>
			<view class="ButCont" v-else-if="type === 5">
				<text class="butRed">待离厂</text>
			</view>
			<view class="ButCont" v-else-if="type === 6">
				<text class="butRed">待进港</text>
			</view>
			<view class="ButCont" v-else-if="type === -1">
				<text class="butRed butDis">已完成</text>
			</view>
		</view>
	</view>
	<!-- end -->
</template>

<script>
export default {
	props:{
		data: {
			type: Object,
			default: () => ({
				TaskId:"4321",
				isoutput:1,
				startAddress:"杭州萧山",
				midAddress:"杭州萧山",
				endAddress:"杭州萧山",
				planDepartureTime:"2024.8.19 12:00:00 今天",
				alreadyPickUp:true,
				boxtype:"40HQ",
				boxweight:"21000KGS",
				message:"only test",
				dispatch:"派单：2024.8.18 00:00",
				day:"今天"
			})
		},
		type: {
			type: Number,
			default: 0
		},
		src:{
			type: String,
			default: '/pages/detail/index'
		}
	},
	
	data(){
		return{
			item : {...this.data},
		};
	},
	
	methods:{
		gotoDetail(item){
			if (this.src) {
			    uni.navigateTo({
					url: `${this.src}?type=${this.type}`,
			    });
		    }
		}
	}
}
</script>

<style lang="scss" scoped>
@import url(@/components/styles/theme.css);	
.card{
		padding: 30rpx 40rpx;
		margin: 30rpx 0;
		border-radius: 20rpx;
		background: var(--neutral-color-white);
		.topCont{
			display: flex;
			justify-content: space-between;
			font-weight: bold;
			line-height: 60rpx;
			font-size: 32rpx;
			color: #2A2929;
			// margin-bottom: 20rpx;
			.label{
				display: inline-block;
				padding: 16rpx 10rpx 12rpx 10rpx;
				border: 2rpx solid #EF4F3F;
				border-radius: 20rpx;
				font-weight: 400;
				font-size: 28rpx;
				line-height: 28rpx;
				color: #EF4F3F;
				letter-spacing: 0.32rpx;
			}
		}
		.otherinfo{
			display: flex;
			justify-content: flex-start;
			font-weight: 400;
			line-height: 60rpx;
			font-size: 28rpx;
			color: #818181;
			margin-bottom: 20rpx;
			.space{
				margin-right:20rpx;
			}
		}
		.addrCont{
			font-weight: 400;
			font-size: 28rpx;
			color: #818181;
			letter-spacing: 0.32rpx;
			padding-bottom: 40rpx;
			margin-bottom: 20rpx;
			border-bottom:  1px solid #EEEEEE;
			position: relative;
			&:before{
				position: absolute;
				left: 18rpx;
				color:var(--neutral-color-white);
				text-align: center;
				content: '';
				display: inline-block;
				width: 0px;
				height: 56%;
				border-left: dashed 2px #D9D9D9;
				border-radius: 11px;
			}
			.startAddr{
				padding-left: 66rpx;
				position: relative;
				margin-bottom: 20rpx;
				line-height: 44rpx;
				&:before{
					position: absolute;
					font-size: 22rpx;
					left: 0;
					color:var(--neutral-color-white);
					text-align: center;
					content: '提';
					display: inline-block;
					width: 22px;
					height: 22px;
					background: #1e131d;
					border-radius: 11px;
				}
			}
			.midAddr{
				padding-left: 66rpx;
				position: relative;
				margin-bottom: 20rpx;
				line-height: 44rpx;
				&:before{
					position: absolute;
					font-size: 22rpx;
					left: 0;
					color:var(--neutral-color-white);
					text-align: center;
					content: '门';
					display: inline-block;
					width: 22px;
					height: 22px;
					background: #22a2c3;
					border-radius: 11px;
				}
			}
			.endAddr{
				padding-left: 66rpx;
				position: relative;
				line-height: 44rpx;
				// height: 44rpx;
				// overflow: hidden;
				&:before{
					font-size: 22rpx;
					position: absolute;
					left: 0;
					color: var(--neutral-color-white);
					text-align: center;
					content: '还';
					display: inline-block;
					width: 22px;
					height: 22px;
					background: #EF4F3F;
					border-radius: 11px;
				}
			}
			.topspace{
				margin-top: 20px;
			}
		}
		.botCont{
			position: relative;
			display: flex;
			justify-content: space-between;
			align-items: center;
			.timeCont{
				.tit{
					font-weight: 400;
					font-size: 28rpx;
					margin-bottom: 20rpx;
					color: #818181;
				}
				.time{
					font-size: 28rpx;
					color: #2A2929;
				}
			}
			.finish{
				display: flex;
				align-items: center;
				.tit{
					margin-bottom: 0;
					margin-right: 18rpx;
				}
			}
			.butRed, .butDis{
				display: inline-block;
				min-width: 80px;
				line-height: 32px;
				background: #EF4F3F;
				border-radius: 16px;
				font-size: var(--font-size-14);
				text-align: center;
				color: var(--neutral-color-white)
			}
			.butDis{
				background: #DEDEDE;
			}
			.butAsh{
				display: inline-block;
				min-width: 80px;
				line-height: 32px;
				background: #EF4F3F;
				border-radius: 16px;
				text-align: center;
				color: var(--neutral-color-white)
			}
		}
	}
</style>
