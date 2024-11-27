/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief
	* @function List:
	******************************************************************************
	* @attention
	*
	*
	* <h2><center>&copy; COPYRIGHT 2021 </center></h2>
	******************************************************************************
	* @History:
	* @Author:
	* @Data:
	* @Version:
*/

#include "queue_entity.h"


/**
 * @brief
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
#ifdef	TEST_MODEL_QUEUE


#define MAX_QUEUE_QSIZE  8u

typedef u32 queue_data_elem;

static sequential_queue_s  Q1 ;
static queue_data_elem  Q1buf[MAX_QUEUE_QSIZE]; /* 隊列 */

typedef struct
{
	QUEUE_RIGHT_FLAG_E empty;
	u16 length;
	queue_data_elem value;
}queue_test_s;

static queue_test_s queue_test =  {QUEUE_FALSE,0u,0u } ;




/*考虙到測試代碼中都包含11.4 條 批量過虙*/
/*PCLINT_ERROS_928*/ /*lint -e928 */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
/*考虙到測試代碼中都包含 該無返回值的過虙 批量過虙*/
/*PCLINT_ERROS*/	/*lint -e534 */ /*忽略該函數的返回結果*/

  /**
 * @brief   若隊列不空，則刪除Q中的某一個元素
 * @param  input: seq : 順序隊列；i表示元素地址; fdo 刪除的方法
 * @param  output:  element: 將頭元素數據傳給外部的變量;
 * @return   是否操作成功
 * @calls
 * @calls by
 * @others
 */
void queue_test_task(void)
{

	u16 i,j;
	u16 start,end ;
	queue_data_elem  l_value;

 	/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
	/*PCLINT_ERROS_960*/ /*lint --e(960) */   /*表達式是函數參數 不能強制隱制轉換*/
	queue_init(&Q1,(sequential_queue_elem*)&Q1buf[0],MAX_QUEUE_QSIZE,sizeof(queue_data_elem));

	queue_test.empty = queue_get_empty(Q1);   /*  初始化隊列 */

	if(queue_test.empty == QUEUE_TRUE)
	{

#if 0

		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa2);
		queue_add_element(&Q1,0xa3);
		queue_add_element(&Q1,0xa4);
		queue_add_element(&Q1,0xa5);

		length = queue_get_length(Q1);

		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);
		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);
		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);
		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);

		queue_add_element(&Q1,0xb1);
		queue_add_element(&Q1,0xb2);
		queue_add_element(&Q1,0xb3);
		queue_add_element(&Q1,0xb4);
		queue_add_element(&Q1,0xb5);

		length = queue_get_length(Q1);

		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);
		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);
		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);

		queue_add_element(&Q1,0xc1);
		queue_add_element(&Q1,0xc2);
		queue_add_element(&Q1,0xc3);
		queue_add_element(&Q1,0xc4);
		queue_add_element(&Q1,0xc5);

		queue_del_element(&Q1,&queue_test.value);
		length = queue_get_length(Q1);


		for(i=0;i<(MAX_QUEUE_QSIZE+5);i++)
		{
		queue_add_element(&Q1,0xa1 + i);
		}


		for(i=Q1.front;i<Q1.rear;i++)
		{
		if( Q1.base[i%MAX_QUEUE_QSIZE] != 0)
		queue_del_element(&Q1,&queue_test.value);
		}


		length = queue_get_length(Q1);

		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);

		queue_del_element(&Q1,&queue_test.value);
		queue_del_element(&Q1,&queue_test.value);
		queue_del_element(&Q1,&queue_test.value);
		queue_del_element(&Q1,&queue_test.value);


		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);
		queue_add_element(&Q1,0xa1);


		length = queue_get_length(Q1);
		if( length == 0u)
		{
			queue_add_element(&Q1,0xa1);
		}

		queue_del_element(&Q1,&queue_test.value);
		queue_del_element(&Q1,&queue_test.value);

#else

		l_value = 0xa1u;
	 	/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		l_value++;
	 	/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		l_value++;
		/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		l_value++;
		/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);


	 	/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
		queue_del_one_element(&Q1,0u,(sequential_queue_elem*)&queue_test.value);
		queue_del_one_element(&Q1,1u,(sequential_queue_elem*)&queue_test.value);
		/*PCLINT_ERROS_928*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
		queue_del_one_element(&Q1,3u,(sequential_queue_elem*)&queue_test.value);

		queue_test.length = queue_get_length(Q1);

		l_value = 0xa1u;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		queue_test.length = queue_get_length(Q1);

		/* queue_del_one_element(&Q1,5,&queue_test.value);  */

		queue_test.value = 9u;
		queue_del_element(&Q1,(sequential_queue_elem*)&queue_test.value);

		queue_del_element(&Q1,(sequential_queue_elem*)&queue_test.value);

		queue_del_element(&Q1,(sequential_queue_elem*)&queue_test.value);

		queue_del_element(&Q1,(sequential_queue_elem*)&queue_test.value);

		l_value = 0xa1u;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		queue_test.length = queue_get_length(Q1);

		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);


		start = Q1.front;

		end = start + queue_get_length(Q1);

#if 1
		for(i=start;i<end;i++)
#else
		//for(i=Q1.front;i<(Q1.front + QueueLength(Q1));i++)
#endif
		{
			j = i%Q1.length;
			if( Q1buf[j] != 0u)
			{
				queue_del_one_element(&Q1,j,(sequential_queue_elem*)&queue_test.value);
				queue_test.length = queue_get_length(Q1);
			}

		}

		l_value = 0xa1u;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		l_value++;
		queue_add_element(&Q1,(sequential_queue_elem*)&l_value);

		queue_test.length = queue_get_length(Q1);
		queue_del_element(&Q1,(sequential_queue_elem*)&queue_test.value);
		queue_del_element(&Q1,(sequential_queue_elem*)&queue_test.value);

		if( queue_test.length == 0u)
		{
			l_value = 0xa1u;
			queue_add_element(&Q1,(sequential_queue_elem*)&l_value);
		}


#endif


	}



}





#endif




