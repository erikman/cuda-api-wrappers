/**
 * @file implementations of methods requiring the definitions of multiple
 * API wrapper classes
 */
#pragma once
#ifndef MULTI_WRAPPER_IMPLS_HPP_
#define MULTI_WRAPPER_IMPLS_HPP_

#include "cuda/api/stream.hpp"
#include "cuda/api/device.hpp"
#include "cuda/api/event.hpp"

namespace cuda {

// device_t methods

template <bool AssumedCurrent>
inline void device_t<AssumedCurrent>::synchronize(event_t& event)
{
	return synchronize_event(event.id());
}

template <bool AssumedCurrent>
inline void device_t<AssumedCurrent>::synchronize(stream_t<detail::do_not_assume_device_is_current>& stream)
{
	return synchronize_stream(stream.id());
}

template <bool AssumedCurrent>
stream_t<AssumedCurrent> device_t<AssumedCurrent>::default_stream() const
{
	// TODO: Perhaps support not-knowing our ID here as well, somehow?
	return stream_t<AssumedCurrent>(id(), stream::default_stream_id);
}

template <bool AssumedCurrent>
stream_t<detail::do_not_assume_device_is_current>
device_t<AssumedCurrent>::create_stream(
	stream::priority_t  priority,
	bool                synchronizes_with_default_stream)
{
	scoped_setter set_device_for_this_scope(id_);
	return stream_t<>(id(), stream::detail::create_on_current_device(
		priority, synchronizes_with_default_stream));
}

template <bool AssumedCurrent>
stream_t<detail::do_not_assume_device_is_current>
device_t<AssumedCurrent>::create_stream(bool synchronizes_with_default_stream)
{
	return create_stream(stream::default_priority, synchronizes_with_default_stream);
}

// event_t methods

device_t<detail::do_not_assume_device_is_current>
event_t::device() const { return cuda::device::get(device_id_); }

// stream_t methods

template <bool AssumesDeviceIsCurrent>
device_t<detail::do_not_assume_device_is_current>
stream_t<AssumesDeviceIsCurrent>::device() const { return cuda::device::get(device_id_); }

} // namespace cuda

#endif /* MULTI_WRAPPER_IMPLS_HPP_ */
