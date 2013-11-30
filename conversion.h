
// CONVERTABILITY TO STD-CONTAINERS	(included as member of basic_range dirivatives)  (shoud re-implement ad CRTP?)

	template<class A, template<class,class> class Ct> 		//  std sequence containers
operator Ct<value_type,A>(){
	Ct<value_type,A>  C(size());
	C.clear();
	C << *this;							// TOFIX: remove dependance on RO

	return std::move(C);
}

	template<class Cmp, class A, template<class, class, class> class Ct> 	//  std::set
operator Ct<value_type,Cmp,A>(){
	Ct<value_type,Cmp,A>  C;
	C << *this;
	return std::move(C);
}

	
	template<class H, class KE, class A, template<class, class, class, class> class Ct> 
operator Ct<value_type,H,KE,A>() {					//  std::unordered_set
	Ct<value_type,H,KE,A>  C;
	C << *this;
	return std::move(C);
}
