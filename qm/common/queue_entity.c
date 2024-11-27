/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief  消息隊列
				順序循環消息隊列   ----  溢出丟棄
					約定:1.隊列尾指針為實際末尾無素的下一位置.   2.浪費一個元素空間,以"隊列頭指
						針在隊列尾指針的下一位置(指環狀的下一位置)上作為隊列呈"滿"狀態的標誌.
						seq為空循環隊列的判定條件為:頭尾重合      seq.front==seq.rear
						seq為滿循環隊列的判定條件為:隊尾趕上隊頭  (seq.rear+1)%MAXSIZE==seq.front
				    注: 申請的隊列空間,buf至少2個.
注: 定義隊列緩存時;可以多加一個地址緩存;但是記住在調用queue_init函數時;參數length的值就是隊列緩存大小值;
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


#include "common_types.h"

#include "queue_entity.h"




/**
* @brief  初始化一個空順序隊列
* @param  input:  seq : 順序隊列；pdata 隊列元素的數據; length隊列長度,unit_sizes 元素的字節長度;
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void queue_init(sequential_queue_s *seq, sequential_queue_elem *pdata, u8 length, u8 unit_sizes) {
    seq->front = 0u;
    seq->rear = 0u;
    seq->base = pdata;
    seq->length = length;
    seq->unit_sizes = unit_sizes;
}

/**
* @brief  清空隊列
* @param  input:  seq : 順序隊列；
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void queue_clear(sequential_queue_s *seq) {
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    seq->front = 0u;
    seq->rear = 0u;
    
    QUEUE_CPU_CRITICAL_EXIT();
}



/**
 * @brief  判斷隊列是否為空;
 * @param  input:  seq : 順序隊列；
 * @param  output:
 * @return   QUEUE_RIGHT_FLAG_E: 若隊列seq為空隊列，則返回ture 否則返回false
 * @calls
 * @calls by
 * @others
 */
QUEUE_RIGHT_FLAG_E queue_get_empty(sequential_queue_s seq) {

    QUEUE_RIGHT_FLAG_E l_resutl;
    
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    /*隊列空的標誌*/
    if (seq.front == seq.rear) {
        l_resutl = QUEUE_TRUE;
    } else {
        l_resutl =  QUEUE_FALSE;
    }
    
    QUEUE_CPU_CRITICAL_EXIT();
    
    return l_resutl;
}


/**
 * @brief  求出隊列不為空元素的數目;
 * @param  input:  seq : 順序隊列；
 * @param  output:
 * @return   長度數目
 * @calls
 * @calls by
 * @others
 */
u16 queue_get_length(sequential_queue_s seq) {

    #ifdef QUEUE_MOD_ENABLE
    u16 length = (seq.length + seq.rear) - seq.front ;
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    length %= seq.length;
    #else
    u16 length ;
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    if (seq.rear >= seq.front) {
        length = seq.rear - seq.front;
    } else {
        #if 0
        length = seq.length + seq.rear - seq.front;
        #else
        length = seq.length + seq.rear;
        length -= seq.front;
        #endif
    }
    #endif
    
    QUEUE_CPU_CRITICAL_EXIT();
    
    return length;
}


/**
* @brief  若隊列不為空，則用e返回seq的隊頭元素
* @param  input: seq : 順序隊列列
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/
QUEUE_OPERATE_FLAG_E queue_get_head(sequential_queue_s *seq, sequential_queue_elem *element) {

    sequential_queue_elem *pdata;
    u8 j;
    QUEUE_OPERATE_FLAG_E l_resutl = QUEUE_OK;
    
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    if (seq->front == seq->rear) {
        l_resutl = QUEUE_ERRO;
    }
    
    if (l_resutl == QUEUE_OK) {
    
        /* 將隊頭元素賦值給e */
        #ifdef QUEUE_MOD_ENABLE
        //base为can1_rx_qbuf的首地址，length为CAN1_QUEUE_RX_DEPTH，unit_sizes为bl_CanRxPdu_t的字节长度
        pdata = seq->base + ((seq->front % seq->length) * seq->unit_sizes) ;
        #else
        pdata = seq->base + ((seq->front) * seq->unit_sizes) ;
        #endif
        
        for (j = 0u; j < seq->unit_sizes; j++) {
            element[j] = *pdata;
            /* 該語句刪除隊列元素; */
            /* *pdata = 0;*/
            pdata += sizeof(sequential_queue_elem);
        }
        
    }
    QUEUE_CPU_CRITICAL_EXIT();
    return l_resutl;
}



/**
 * @brief  若隊列不為空，則用e返回seq的隊頭元素
 * @param  input: seq : 順序隊列；
 * @param  output:  element: 將頭元素數據傳給外部的變量;
 * @return   是否操作成功
 * @calls
 * @calls by
 * @others
 */
QUEUE_OPERATE_FLAG_E queue_get_element(sequential_queue_s seq, u8 i, sequential_queue_elem *element) {
    sequential_queue_elem *pdata;
    u8 j;
    QUEUE_OPERATE_FLAG_E l_resutl = QUEUE_OK;
    
    #ifdef QUEUE_MOD_ENABLE
    #else
    u16 tmp_i ;
    #endif
    
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    if (seq.front == seq.rear) {
        l_resutl = QUEUE_ERRO;
    }
    
    if (l_resutl == QUEUE_OK) {
    
        #ifdef QUEUE_MOD_ENABLE
    
        /* 將隊頭元素賦值給e */
        pdata = seq.base + ((i % seq.length) * seq.unit_sizes) ;
        #else
        if (i < seq.length) {
            tmp_i  = i;
        } else {
            tmp_i  = i - seq.length ;
        }
        
        pdata = seq.base + (tmp_i * seq.unit_sizes) ;
        
        #endif
        
        for (j = 0u; j < seq.unit_sizes; j++) {
            element[j] = *pdata;
            /* 該語句刪除隊列元素; */
            /*  *pdata = 0; */
            pdata += sizeof(sequential_queue_elem);
        }
        
    } else {
    
    }
    QUEUE_CPU_CRITICAL_EXIT();
    
    return l_resutl;
    
}




/**
 * @brief   若隊列未滿，則插入元素e為seq新的隊尾元素
 * @param  input: seq : 順序隊列；
 * @param  output:  element: 將頭元素數據傳給外部的變量;
 * @return   是否操作成功
 * @calls
 * @calls by
 * @others
 */
/*lint -efunc(818,queue_add_element) */ /*（建議）16.7 函數的指針參數 如果不是用來修改的話，建議聲明指向 const 類型*/
QUEUE_OPERATE_FLAG_E queue_add_element(sequential_queue_s *seq, const sequential_queue_elem *element) {

    sequential_queue_elem *pdata;
    u8 i;
    QUEUE_OPERATE_FLAG_E l_resutl = QUEUE_OK;
    
    #ifdef QUEUE_MOD_ENABLE
    #else
    u16 tmp_rear ;
    #endif
    
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    #ifdef QUEUE_MOD_ENABLE
    /* 隊列滿的標誌 */
    /* -----------------------------------------------------------------------A12
    初始化时，rear，front=0，length=8。
    ----------------------------------------------------------------------- */
    if (((seq->rear + 1u) % seq->length) == seq->front)
    #else
    tmp_rear = seq->rear + 1u;
    if (tmp_rear >= seq->length) {
        tmp_rear = 0u;
    }
    if (tmp_rear == seq->front)
    #endif
    {
        l_resutl = QUEUE_ERRO;
        
    }
    
    if (l_resutl == QUEUE_OK) {
    
        /*將元素e賦值給隊尾*/
        #ifdef QUEUE_MOD_ENABLE
        pdata = seq->base + ((seq->rear % seq->length) * seq->unit_sizes) ;
        #else
        pdata = seq->base + ((seq->rear) * seq->unit_sizes) ;
        #endif
        
        for (i = 0u; i < seq->unit_sizes; i++) {
            *pdata = element[i];
            pdata += sizeof(sequential_queue_elem);
        }
        
        /*seq->base[seq->rear%seq->length]=element;*/
        
        /* rear指針向後移一位，若到最後就轉到 數組頭部*/
        
        #ifdef QUEUE_MOD_ENABLE
        
        seq->rear = (seq->rear + 1u) % seq->length;
        
        #else
        
        seq->rear++;
        if (seq->rear >= seq->length) {
            seq->rear = 0u;
        }
        
        #endif
        
        
        
    }
    
    QUEUE_CPU_CRITICAL_EXIT();
    
    return l_resutl;
    
}



/**
* @brief   若隊列未滿，則刪除元素e為seq新的隊尾元素
* @param  input: seq : 順序隊列；
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/
QUEUE_OPERATE_FLAG_E queue_del_element(sequential_queue_s *seq, sequential_queue_elem *element) {
    QUEUE_OPERATE_FLAG_E l_resutl = QUEUE_OK;
    sequential_queue_elem *pdata;
    u8 i;
    
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    
    /* 隊列空的判斷 */
    if (seq->rear == seq->front) {
        l_resutl = QUEUE_ERRO;
    }
    
    if (l_resutl == QUEUE_OK) {
    
        /* 將隊頭元素賦值給e */
        #ifdef QUEUE_MOD_ENABLE
        pdata = seq->base + ((seq->front % seq->length) * seq->unit_sizes) ;
        #else
        pdata = seq->base + ((seq->front) * seq->unit_sizes);
        #endif
        
        for (i = 0u; i < seq->unit_sizes; i++) {
            element[i] = *pdata;
            /* 該語句刪除隊列元素; */
            *pdata = 0u;
            pdata += sizeof(sequential_queue_elem);
        }
        /* *element = seq->base[seq->front]; */
        
        /* 該語句刪除隊列元素; */
        /*seq->base[seq->front] = 0;  */
        
        #ifdef QUEUE_MOD_ENABLE
        
        /* front 指針向後移一位置 */
        seq->front = (seq->front + 1u) % seq->length;
        
        #else
        
        seq->front++;
        if (seq->front >= seq->length) {
            seq->front = 0u;
        }
        
        #endif
        
        
    }
    
    QUEUE_CPU_CRITICAL_EXIT();
    
    return l_resutl;
    
}


#if 1

/**
* @brief   若隊列不空，則刪除Q中的某一個元素
* @param  input: seq : 順序隊列；i表示元素地址;
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/
QUEUE_OPERATE_FLAG_E queue_del_one_element(sequential_queue_s *seq, u16 i, sequential_queue_elem *element) {
    QUEUE_OPERATE_FLAG_E l_resutl = QUEUE_OK;
    sequential_queue_elem *pdata;
    u8 j;
    #ifdef QUEUE_MOD_ENABLE
    
    #else
    u16 tmp_i;
    #endif
    
    QUEUE_CPU_SR_ALLOC();  /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    
    /* 隊列空的判斷 */
    if (seq->rear == seq->front) {
        l_resutl = QUEUE_ERRO;
    }
    
    
    if (l_resutl == QUEUE_OK) {
    
        l_resutl = QUEUE_ERRO;
        
        #if 0
        /* 將隊頭元素賦值給e */
        *element = seq->base[i % seq->length];
        /* 該語句刪除隊列元素; */
        seq->base[i % seq->length] = 0;
        #else
        
        /* 將隊頭元素賦值給e */
        #ifdef QUEUE_MOD_ENABLE
        
        pdata = seq->base + ((i % seq->length) * seq->unit_sizes) ;
        
        #else
        
        if (i < seq->length) {
            tmp_i  = i;
        } else {
            tmp_i  = i - seq->length ;
        
        }
        
        pdata = seq->base + (tmp_i * seq->unit_sizes);
        
        #endif
        
        
        for (j = 0u; j < seq->unit_sizes; j++) {
            element[j] = *pdata;
            /* 該語句刪除隊列元素; */
            *pdata = 0u;
            pdata += sizeof(sequential_queue_elem);
        }
        
        #endif
        
        
        
        
        #ifdef QUEUE_MOD_ENABLE
        
        if ((seq->front % seq->length) == (i % seq->length))
        
        #else
        
        /* 如果要刪除的是隊頭 */
        if (seq->front == tmp_i)
        
        #endif
        
        {
        
            #ifdef QUEUE_MOD_ENABLE
        
            seq->front = (seq->front + 1u) % seq->length;
            
            #else
            
            /* front 指針向後移一位置 */
            seq->front++;
            if (seq->front >= seq->length) {
                seq->front = 0u;
            }
            
            #endif
            
            l_resutl = QUEUE_OK;
            
            
        }
        
        if (l_resutl == QUEUE_ERRO) {
        
            /* 如果要刪除的是隊尾 */
            #ifdef QUEUE_MOD_ENABLE
            if ((((seq->rear + seq->length) - 1u) % seq->length) == (i % seq->length))
            #else
            tmp_i++;
            if (tmp_i >= seq->length) {
                tmp_i = 0u;
            }
            if (seq->rear == tmp_i)
            #endif
            {
            
                #ifdef QUEUE_MOD_ENABLE
            
                /* rear 指針向前移一位置 */
                seq->rear = ((seq->rear + seq->length) - 1u) % seq->length;
                
                #else
                
                if (seq->rear > 0u) {
                    seq->rear--;
                } else {
                    seq->rear = seq->length - 1u;
                }
                
                #endif
                
                l_resutl = QUEUE_OK;
                
            }
        }
        
        /*lint -e(838) */ /*局部變量只作臨界交換用;所以提示沒作用*/
        /* l_resutl = QUEUE_OK; */
        
        
    }
    
    
    QUEUE_CPU_CRITICAL_EXIT();
    
    return l_resutl;
    
    
}



/**
* @brief   若隊列不空，則刪除Q中的某一個元素
* @param  input: seq : 順序隊列；i表示元素地址; fdo 刪除的方法
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/
QUEUE_OPERATE_FLAG_E queue_del_one_element_by_flag(sequential_queue_s *seq, u16 i, sequential_queue_elem *element, QUEUE_DEL_FLAG_E fdo) {
    QUEUE_OPERATE_FLAG_E l_resutl = QUEUE_OK;
    sequential_queue_elem *pdata;
    u8 j;
    u8 flag_del = 0u;
    
    #ifdef QUEUE_MOD_ENABLE
    
    #else
    u16 tmp_i;
    #endif
    
    QUEUE_CPU_SR_ALLOC(); /*PRQA S 2981*/
    QUEUE_CPU_CRITICAL_ENTER();
    
    
    
    /* 隊列空的判斷 */
    if (seq->rear == seq->front) {
        *element = 0u;
        
        /* 開啟總中斷 */
        l_resutl =  QUEUE_ERRO;
        
    }
    
    if (l_resutl  == QUEUE_OK) {
    
        /* 複製元素*/
        #ifdef QUEUE_MOD_ENABLE
        
        pdata = seq->base + ((i % seq->length) * seq->unit_sizes);
        
        #else
        
        if (i < seq->length) {
            tmp_i  = i;
        } else {
            tmp_i  = i - seq->length ;
        }
        
        pdata = seq->base + (tmp_i * seq->unit_sizes) ;
        
        #endif
        
        for (j = 0u; j < seq->unit_sizes; j++) {
            element[j] = *pdata;
            
            /* 該語句刪除隊列元素; */
            if (fdo == QUEUE_DEL) {
                *pdata = 0u;
            } else if ((fdo == QUEUE_MINUS) && (j == 0u)) {
                if (*pdata != 0u) {
                    *pdata = *pdata - 1u;
                    /*重新運行的次數被清為0時；將隊列元素刪除*/
                    if (*pdata == 0u) {
                        flag_del = 1u;
                    }
                }
            } else { /*PRQA S 2013*/
            
            }
            
            pdata += sizeof(sequential_queue_elem);
            
        }
        
        l_resutl = QUEUE_ERRO;
        /*滿足刪除隊列的標誌*/
        if ((fdo == QUEUE_DEL) || (flag_del == 1u)) {
        
            #ifdef QUEUE_MOD_ENABLE
            /* 如果要刪除的是隊頭 */
            if ((seq->front % seq->length) == (i % seq->length))
            #else
            if (seq->front == tmp_i)
            #endif
            {
                /* front 指針向後移一位置 */
                
                #ifdef QUEUE_MOD_ENABLE
                
                seq->front = (seq->front + 1u) % seq->length;
                
                #else
                
                /* front 指針向後移一位置 */
                seq->front++;
                if (seq->front >= seq->length) {
                    seq->front = 0u;
                }
                
                #endif
                
                l_resutl = QUEUE_OK;
                
            }
            
            if (l_resutl == QUEUE_ERRO) {
            
                /* 如果要刪除的是隊尾 */
                #ifdef QUEUE_MOD_ENABLE
                if ((((seq->rear + seq->length) - 1u) % seq->length) == (i % seq->length))
                #else
                tmp_i++;
                if (tmp_i >= seq->length) {
                    tmp_i = 0u;
                }
                if (seq->rear == tmp_i)
                #endif
                {
                
                    #ifdef QUEUE_MOD_ENABLE
                
                    /* rear 指針向前移一位置 */
                    seq->rear = ((seq->rear + seq->length) - 1u) % seq->length;
                    
                    #else
                    
                    if (seq->rear > 0u) {
                        seq->rear--;
                    } else {
                        seq->rear = seq->length - 1u;
                    }
                    
                    #endif
                    
                    l_resutl =  QUEUE_OK;
                    
                }
                
            }
        }
        
        
    }
    
    QUEUE_CPU_CRITICAL_EXIT();
    
    return l_resutl;
    
}


#endif






















