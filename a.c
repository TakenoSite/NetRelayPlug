#include "a.h"
#include "d.h"
#include "f.h"
#include "g.h"
#include "z.h"

#define EXSAMPLE_IPV4 "1.1.1.1"

//node_table *index_head, index_array_null;
//vec2 *vec2_head, vec2_array_null;


int create_table(vec2 *vec_head, node_table *index_head)
{	

	char *addr = (char*)malloc(IPV4_BUF);
	strcpy(addr, EXSAMPLE_IPV4);
	
	vec_head->next   = NULL;
	index_head->next = NULL;
	
	if(create_addr_array(vec_head, 1000, addr) && insert_index_table(index_head, vec_head, 0)){
		deletes(index_head, 0);
		return 0;
	}

	return -1;

}


int create_session(CREATE_SESSION *c_s)
{
	//bool gate = true;
		
	int counter  = 0;
	size_t msize = RECV_MSG_BUF-1, rsize=0;

	if(sock_bind(c_s->udp_sock, c_s->addr) < 0)
	{
#ifdef E_LOG
		log_error("[!] create_session : bind");
#endif 
		return 1;
	}
#ifdef LOG
	printf("[*] create session ... \n\n");
	printconfig();
#endif 
	
	while(SESSION_GATE)
	{
		Memset(&c_s->sinfo, sizeof(c_s->sinfo));
		Memset(c_s->recv_msg_buf, sizeof(c_s->recv_msg_buf));
		
#ifdef D_LOG 
#ifdef SHOWTABLE
		node_table *index = c_s->index_head;
		print_array(index, "\nnomal_index_head");			
		print_array(c_s->raw_node_index_head, "raw_node_index_head");
#endif
#endif // D_LOG 

		c_s->addrlen = sizeof(c_s->sinfo);
		recvfrom(c_s->udp_sock, c_s->recv_msg_buf, (msize), 0, (struct sockaddr *)&c_s->sinfo, &c_s->addrlen);

		if(strlen(c_s->recv_msg_buf) >= msize)
		{
#ifdef LOG
			log_warning("[-] create_session : OverSize");
#endif 
			continue;
		}

#ifdef D_LOG
		rsize = strlen(c_s->recv_msg_buf);
		printf("[+] create_session : MSG :: %s :: LEN :: %ld\n", c_s->recv_msg_buf, rsize);
	
		printf("[%d] %s\n", counter, LOGN_LINE);
		counter += 1;
#endif 
	
	msg_parse(
			c_s->recv_msg_buf,
			c_s->sinfo, 
			c_s->udp_sock, 
			c_s->index_head, 
			c_s->raw_node_index_head, 
			c_s->raw_node_body_head, 
			c_s->vec2_head
			);
	}
	
	/* finish */ 
	return 0;
}
