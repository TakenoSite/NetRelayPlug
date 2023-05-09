#include "b.h"

// ノードテーブルを更新
int node_update_table(struct sockaddr_in addr, int sock, int sport, node_table *table, vec2 *vec2_info){ // node_update
	
	char *update_host = (char*)malloc(IPV4_BUF);
	inet_ntop(AF_INET, &addr.sin_addr, update_host, IPV4_BUF);
	
	int src_port;
	int check;
	
	// 要素を探査する
	check = table_search(table, update_host);
		
	if(check == -1)
	{
		sendTo(sock, addr, BIND_PROTOCOL_FALSE_MSG);
		free(update_host);
		return -1;
	}

	if(sport == 0){
		src_port = htons(addr.sin_port);
	}else{
		src_port = sport;		
	}

	// vec2を生成しIndex table に追加する
	if(create_addr_array(vec2_info, src_port,update_host))
	{	
		sendTo(sock, addr, BIND_PROTOCOL_TRUE_MSG);
		raplacement(table, vec2_info, check);
		
		return 0;
	}

	sendTo(sock, addr, BIND_PROTOCOL_FALSE_MSG);
	free(update_host);
		
	return -2;
}


//ノードを追加する
int node_add_table(struct sockaddr_in addr, int sock, int sport, node_table *table, vec2 *vec2_info)
{
	
	int port;
	char *node_host = (char*)malloc(IPV4_BUF);
	if(node_host == NULL)
	{
#ifdef E_LOG
		// erorr
#endif
		return -3;
	}
	else
	{
		Memset(node_host, IPV4_BUF);
	}

	inet_ntop(AF_INET, &addr.sin_addr,
			node_host, IPV4_BUF);
	
	// 重複してないか確認する。
	int check;
	check = table_search(table, node_host);
	if(check != -1)
	{ 
		
		// true 重複する場合自動的に更新する 
		node_update_table(addr, sock, sport, table, vec2_info);
		
		// false ノード情報が重複する場合、エラーをレスポンスしてクローズ
		//sendTo(sock, addr, BIND_ERROR_RESPONSE_MSG);
		free(node_host);
		return -1;
	};
	
	if(sport == 0)
	{
		port = htons(addr.sin_port);
	}
	else
	{
		port = sport;
	}
	
	//　ノード情報をindex tableに追加する
	if(create_addr_array(vec2_info, port, node_host) && push_back(table, vec2_info))
	{	
		sendTo(sock, addr, BIND_PROTOCOL_TRUE_MSG);
		return 0;
	}
	else
	{
		sendTo(sock, addr, BIND_PROTOCOL_FALSE_MSG);
		free(node_host);
		return -2;
	}
}



// Delete Addr
int node_delete_table(struct sockaddr_in addr, int sock, node_table *index_head, node_table *raw_node_index_head) // old to delete_node
{
	char delete_host[IPV4_BUF];
	int search_status=0;

	memset(delete_host, 0, IPV4_BUF);
	inet_ntop(AF_INET, &addr.sin_addr, delete_host, IPV4_BUF);
	
	search_status = table_search(index_head, delete_host);
	if(search_status < 0)
	{
		goto TABLE2;	
	}
	else
	{
		// nodeをdeleteする
		if(safety_array_access(index_head, search_status))
		{
			deletes(index_head, search_status);
		}
		else
		{
			goto END;
		}
	}

TABLE2:
	search_status = table_search(raw_node_index_head, delete_host);
	if(search_status < 0)
	{
		goto END;
	}
	else
	{
		if(safety_array_access(raw_node_index_head, search_status))
		{
			deletes(raw_node_index_head, search_status);
		}
		else
		{
			goto END;
		}
	}

	
END:
	sendTo(sock, addr, DELETE_NONE_RESPONSE_MSG);
	return -1;
}




