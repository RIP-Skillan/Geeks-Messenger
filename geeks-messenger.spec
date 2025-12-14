Name:           geeks-messenger
Version:        1.0.0
Release:        1%{?dist}
Summary:        A C++ CLI Chat Client and Server

License:        MIT
URL:            https://github.com/RIP-Skillan/Geeks-Messenger
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  boost-devel
BuildRequires:  protobuf-devel
BuildRequires:  protobuf-compiler
Requires:       boost-system
Requires:       protobuf

%description
Geeks Messenger is a simple C++ CLI chat application featuring a client and server architecture.
It uses Boost.Asio for networking and Protocol Buffers for message serialization.

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%license LICENSE
%doc README.md
%{_bindir}/client_app
%{_bindir}/server_app

%changelog
* Sun Dec 14 2025 Evan Manto <ripskillan312004@gmail.com> - 1.0.0-1
- Initial package
