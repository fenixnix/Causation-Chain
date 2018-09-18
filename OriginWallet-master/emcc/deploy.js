
function getDeploy(pubkey,name,code,arg,sign){
	data = 'deploy$'+pubkey+'$'+name+'$'+code+'$'+arg;
	block = sign+'`'+data
	return block
}
