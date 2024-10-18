#pragma once
#ifndef U_ENTITY_HPP
#define U_ENTITY_HPP

class entity
{
public:
	void run( );
}; inline std::unique_ptr<entity> u_entity = std::make_unique<entity>( );

#endif