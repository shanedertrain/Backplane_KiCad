/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2023 Jon Evans <jon@craftyjon.com>
 * Copyright The KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <api/api_handler.h>
#include <wx/wx.h>

using kiapi::common::ApiRequest, kiapi::common::ApiResponse, kiapi::common::ApiResponseStatus;


const wxString API_HANDLER::m_defaultCommitMessage = _( "Modification from API" );


API_RESULT API_HANDLER::Handle( ApiRequest& aMsg )
{
    ApiResponseStatus status;

    if( !aMsg.has_message() )
    {
        status.set_status( ApiStatusCode::AS_BAD_REQUEST );
        status.set_error_message( "request has no inner message" );
        return tl::unexpected( status );
    }

    std::string typeName;
    bool debugOn = api_handler_debug::enabled();

    if( !google::protobuf::Any::ParseAnyTypeUrl( aMsg.message().type_url(), &typeName ) )
    {
        status.set_status( ApiStatusCode::AS_BAD_REQUEST );
        status.set_error_message( "could not parse inner message type" );
        return tl::unexpected( status );
    }

    if( debugOn )
    {
        std::string typeUrl = aMsg.message().type_url();
        std::fprintf( stderr,
                      "[api-handler-debug] Handle: this=%p &m_handlers=%p size=%zu "
                      "typeUrl=\"%s\" lookupKey=\"%s\" lookupKeyLen=%zu lookupKeyHex=%s\n",
                      static_cast<void*>( this ), static_cast<void*>( &m_handlers ), m_handlers.size(),
                      typeUrl.c_str(), typeName.c_str(), typeName.size(),
                      api_handler_debug::hexBytes( typeName ).c_str() );
        for( const auto& [key, unused] : m_handlers )
        {
            std::fprintf( stderr, "[api-handler-debug]   key=\"%s\" keyLen=%zu keyHex=%s\n",
                          key.c_str(), key.size(), api_handler_debug::hexBytes( key ).c_str() );
        }
        std::fflush( stderr );
    }

    auto it = m_handlers.find( typeName );

    if( debugOn )
    {
        std::fprintf( stderr, "[api-handler-debug] Handle: find() %s\n",
                      ( it != m_handlers.end() ) ? "SUCCESS" : "FAILED" );
        std::fflush( stderr );
    }

    if( it != m_handlers.end() )
    {
        REQUEST_HANDLER& handler = it->second;
        return handler( aMsg );
    }

    status.set_status( ApiStatusCode::AS_UNHANDLED );
    // This response is used internally; no need for an error message
    return tl::unexpected( status );
}
